/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 19:28:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

static void	check_is_dir(char *filepath)
{
	struct stat	stat_buf;
	int			status;
	
	status = stat(filepath, &stat_buf);
	if (status != 0)
		return ;
	if (stat_buf.st_mode & S_IFDIR)
	{
		fprintf(stderr, "minishell: %s: Is a directory\n",
				filepath);
		errno = EISDIR;
	}
}

static void	check_file_access(char *filepath)
{
	if (access(filepath, F_OK) || access(filepath, X_OK))
	{
		if (errno == EACCES)
			fprintf(stderr, "minishell: %s: Permission denied\n", filepath);
		if (errno == ENOENT)
			fprintf(stderr, "minishell: %s: No such file or directory\n", filepath);
	}
	else
		check_is_dir(filepath);
}

void	exit_fork_clean(t_node *node, char **char_envs, char *path)
{
	char	*is_path;

	free(path);
	free_string_array(char_envs);
	if (!node->cmd->args[0])
		return ;
	is_path = ft_strrchr(node->cmd->args[0], '/');
	if (!ft_strncmp(node->cmd->args[0], ".", 2))
	{
		ft_putstr_fd("minishell: filename argument required", 2);
		exit(2);
	}
	errno = 0;
	if (is_path)
	{
		check_file_access(node->cmd->args[0]);
	}
	else
		fprintf(stderr, "minishell: %s: command not found\n",
			node->cmd->args[0]);
	if (errno == EACCES || errno == EISDIR)
		exit(126);
	exit(127);
}

static pid_t	exec_command_fork(t_node *node, t_list **envs)
{
	pid_t	pid;
	char	**char_envs;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		path = get_command_path(node->cmd->args[0], *envs);
		char_envs = (char **)reconstruct_envs(*envs);
		if (node->fd_in != STDIN_FILENO)
		{
			dup2(node->fd_in, 0);
			close(node->fd_in);
		}
		if (node->fd_out != STDOUT_FILENO)
		{
			dup2(node->fd_out, 1);
			close(node->fd_out);
		}
		execve(path, node->cmd->args, char_envs);
		exit_fork_clean(node, char_envs, path);
	}
	return (pid);
}

int	exec_command(t_node *node, t_list **envs, t_ctx *ctx)
{
	int		status;
	pid_t	pid;

	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
	{
		cleanup_node_fds(node);
		return (1);
	}
	if (resolve_redirs(node, *envs, ctx))
		return (1);
	expand_cmd_args(node, envs, ctx);
	if (is_builtin(node->cmd))
	{
		status = call_builtin(node, envs, ctx);
		cleanup_node_fds(node);
		return (status);
	}
	pid = exec_command_fork(node, envs);
	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	if (node->fd_out != STDOUT_FILENO)
		close(node->fd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
