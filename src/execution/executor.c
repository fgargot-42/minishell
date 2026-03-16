/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/16 14:47:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filepath, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		errno = EISDIR;
	}
}

static void	check_file_access(char *filepath)
{
	if (access(filepath, F_OK) || access(filepath, X_OK))
	{
		if (errno == EACCES)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filepath, 2);
			ft_putstr_fd(": Permission denied\n", 2);
		}
		if (errno == ENOENT)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filepath, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}
	else
		check_is_dir(filepath);
}

void	exit_fork_clean(t_node *node, char **char_envs, char *path,
			t_ctx *ctx)
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
		check_file_access(node->cmd->args[0]);
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	free_tree(ctx->cmd_tree);
	if (errno == EACCES || errno == EISDIR)
		exit(126);
	exit(127);
}

static int	exec_command_fork(t_node *node, t_list **envs, t_ctx *ctx)
{
	pid_t	pid;
	char	**char_envs;
	char	*path;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGPIPE, sigpipe_handler);
		path = get_command_path(node->cmd->args[0], *envs);
		char_envs = (char **)reconstruct_envs(*envs);
		ft_lstclear(envs, env_free);
		apply_redirect(node->fd_in, STDIN_FILENO);
		apply_redirect(node->fd_out, STDOUT_FILENO);
		execve(path, node->cmd->args, char_envs);
		exit_fork_clean(node, char_envs, path, ctx);
	}
	if (pid > 0)
		waitpid(pid, &status, 0);
	return (status);
}

int	exec_command(t_node *node, t_node *parent, t_list **envs, t_ctx *ctx)
{
	int		status;

	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
	{
		cleanup_node_fds(node, parent);
		return (0);
	}
	expand_cmd_args(node, envs, ctx);
	if (!node->cmd->args || !node->cmd->args[0])
	{
		cleanup_node_fds(node, parent);
		return (0);
	}
	if (is_builtin(node->cmd))
	{
		status = call_builtin(node, envs, ctx);
		cleanup_node_fds(node, parent);
		return (status);
	}
	status = exec_command_fork(node, envs, ctx);
	cleanup_node_fds(node, parent);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
