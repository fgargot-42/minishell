/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/09 19:55:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char	*find_in_path(char *cmd)
{
	const char	*paths[] = {"/bin/", "/usr/bin/", "/usr/local/bin/", NULL};
	char		*full_path;
	int			i;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (strdup(cmd));
	while (paths[i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(cmd) + 1);
		strcpy(full_path, paths[i]);
		strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static void	expand_cmd_args(t_node *node, t_list **envs, t_ctx *ctx)
{
	int		i;

	i = 1;
	while (node->cmd->args[i])
	{
		if (!(node->cmd->quote_type[i] == QUOTE_SINGLE))
			expand_var(node->cmd, i, *envs, ctx);
		i++;
	}
}

static pid_t	exec_command_fork(t_node *node, t_list **envs)
{
	pid_t		pid;
	char	**char_envs;
	char		*path;

	pid = fork();
	if (pid == 0)
	{
		path = find_in_path(node->cmd->args[0]);
		char_envs = (char**)reconstruct_envs(*envs);
		if (DEBUG)
			printf("%p\n", node);
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
		free(path);
		free_string_array(char_envs);
		fprintf(stderr, "minishell: %s: command not found\n", node->cmd->args[0]);
		exit(127);
	}
	return (pid);
}

int	exec_command(t_node *node, t_list **envs, t_ctx *ctx)
{
	int		status;
	pid_t	pid;

	if (DEBUG)
		print_str_list(node->cmd->args);
	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
		return (1);
	if (resolve_redirs(node, *envs, ctx))
		return (1);
	if (DEBUG)
		fprintf(stderr, "debug: fd_in=%d, fd_out=%d\n", node->fd_in, node->fd_out);
	expand_cmd_args(node, envs, ctx);
	if (is_builtin(node->cmd))
	{
		if (DEBUG)
			fprintf(stderr, "DEBUG: RUNNING %s as BUILTIN\n", node->cmd->args[0]);
		return (call_builtin(node, envs, ctx));
	}
	if (DEBUG)
		fprintf(stderr, "DEBUG: RUNNING %s as EXTERNAL\n", node->cmd->args[0]);
	pid = exec_command_fork(node, envs);
	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	if (node->fd_out != STDOUT_FILENO)
		close(node->fd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (42);
}
