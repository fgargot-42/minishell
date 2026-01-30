/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/30 19:56:37 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

char	*find_in_path(char *cmd)
{
	const char	*paths[] = {
		"/bin/",
		"/usr/bin/",
		"/usr/local/bin/",
		NULL
	};
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

int	exec_command(t_cmd *cmd, t_list **envs)
{
	int		fd_in;
	int		fd_out;
	fd_in = -1;
	fd_out = -1;
	pid_t	pid;
	int		status;
	char	*path;
	const char	**char_envs = reconstruct_envs(*envs);


	if (DEBUG)
		print_str_list(cmd->args);
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	// check les buitlitns ici
	

	for (int i = 0; cmd->args[i]; i++)
	{

		char *expanded = expand_var(cmd->args[i], *envs);
		if (expanded != cmd->args[i])
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
	}

	if (is_builtin(cmd))
		return (call_builtin(cmd, envs));

	path = find_in_path(cmd->args[0]);
	pid = fork();
	if (pid == 0) //enfant
	{

		/*if (!path)
		{
			fprintf(stderr, "%s cmd not found\n", cmd->args[0]);
			exit(127);
		}*/
		while (cmd->redirs)
		{
			if (cmd->redirs->type == TOKEN_REDIR_IN)
			{
				fd_in = open(cmd->redirs->file, O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
			}
			if (cmd->redirs->type == TOKEN_REDIR_OUT)
			{
				fd_out = open(cmd->redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(fd_out, STDOUT_FILENO);
			}
			if (cmd->redirs->type == TOKEN_APPEND)
			{
				fd_out = open(cmd->redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				dup2(fd_out, STDOUT_FILENO);
			}
			cmd->redirs = cmd->redirs->next;
		}
		execve(path, cmd->args, (char *const*)char_envs);
		exit(127);
	}

	// FREE LE CHAR ENVS
	waitpid(pid, &status, 0);
	free(path);

	if (fd_in != -1)
		close(fd_in);
	if (fd_out != -1)
		close(fd_out);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}
