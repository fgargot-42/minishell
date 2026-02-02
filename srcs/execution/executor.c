/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 01:58:47 by mabarrer         ###   ########.fr       */
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

int	exec_command(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	int			fd_in;
	int			fd_out;
	char		*path;
	const char	**char_envs = reconstruct_envs(*envs);
	int			pid;
	char		*expanded;
	int			status;

	fd_in = -1;
	fd_out = -1;
	if (DEBUG)
		print_str_list(cmd->args);
	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	// check les buitlitns ici
	pid = fork();
	ctx->waitlist[ctx->last++] = pid;
	if (pid == 0)
	{
		for (int i = 0; cmd->args[i]; i++)
		{
			expanded = expand_var(cmd->args[i], *envs);
			if (expanded != cmd->args[i])
			{
				free(cmd->args[i]);
				cmd->args[i] = expanded;
			}
		}
		if (is_builtin(cmd))
			return (call_builtin(cmd, envs));
		path = find_in_path(cmd->args[0]);
		if (!path)
		{
			fprintf(stderr, "%s : command not found\n", cmd->args[0]);
			return (127);
		}
		while (cmd->redirs)
		{
			if (cmd->redirs->type == TOKEN_REDIR_IN)
			{
				fd_in = open(cmd->redirs->file, O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
			}
			if (cmd->redirs->type == TOKEN_REDIR_OUT)
			{
				fd_out = open(cmd->redirs->file, O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				dup2(fd_out, STDOUT_FILENO);
			}
			if (cmd->redirs->type == TOKEN_APPEND)
			{
				fd_out = open(cmd->redirs->file, O_WRONLY | O_CREAT | O_APPEND,
						0644);
				dup2(fd_out, STDOUT_FILENO);
			}
			cmd->redirs = cmd->redirs->next;
		}
		execve(path, cmd->args, (char *const *)char_envs);
		exit(42);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (42);
}
