/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 17:49:04 by fgargot          ###   ########.fr       */
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

int	exec_command(t_node *node, t_list **envs)
{
	pid_t	pid;
	int		status;
	char	*path;
	const char	**char_envs = reconstruct_envs(*envs);

	if (DEBUG)
		print_str_list(node->cmd->args);
	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
		return (0);
	if (resolve_redirs(node))
		return (1);
	// check les buitlitns ici
	
	for (int i = 1; node->cmd->args[i]; i++)
	{

		char *expanded = expand_var(node->cmd->args[i], *envs);
		if (expanded != node->cmd->args[i])
		{
			free(node->cmd->args[i]);
			node->cmd->args[i] = expanded;
		}
	}

	if (is_builtin(node->cmd))
		return (call_builtin(node->cmd, envs));

	path = find_in_path(node->cmd->args[0]);
	pid = fork();
	if (pid == 0) //enfant
	{
		if (node->fd_in != STDIN_FILENO)
		{
			dup2(node->fd_in, 0);
			close(node->fd_in);
		}
		if(node->fd_out != STDOUT_FILENO)
		{
			dup2(node->fd_out, 1);
			close(node->fd_out);
		}
		execve(path, node->cmd->args, (char *const*)char_envs);
		exit(127);
	}
	
	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	if(node->fd_out != STDOUT_FILENO)
		close(node->fd_out);
	// FREE LE CHAR ENVS
	waitpid(pid, &status, 0);
	free(path);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}
