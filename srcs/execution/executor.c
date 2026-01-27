/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 21:35:13 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int	exec_command(t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	// check les buitlitns ici
	if (is_builtin(cmd))
	{
		printf("1 -> %s", cmd->args[0]);
		call_builtin(cmd);
	}
	path = find_in_path(cmd->args[0]);
	if (!path)
	{
		fprintf(stderr, "%s cmd not found\n", cmd->args[0]);
		return (127);
	}
	pid = fork();
	if (pid == 0) //enfant
		execve(path, cmd->args, NULL);
	waitpid(pid, &status, 0);
	free(path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}
