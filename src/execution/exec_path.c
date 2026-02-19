/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:18:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 18:13:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

static char	*find_exec_path(char *cmd, char **path_array)
{
	int		i;
	char	*path_test;

	i = 0;
	while (path_array[i])
	{
		path_test = ft_strjoin_chr(path_array[i], cmd, '/');
		if (path_test)
		{
			if (access(path_test, X_OK) >= 0)
				return (path_test);
			free(path_test);
		}
		i++;
	}
	path_test = ft_strdup(cmd);
	return (path_test);
}

char	*get_command_path(char *cmd, t_list *env)
{
	char	**path_array;
	char	*cmd_out;
	t_list	*path_env;

	if (!cmd)
		return (NULL);
	cmd_out = ft_strdup(cmd);
	if (!env)
		return (cmd_out);
	if (ft_strchr(cmd, '/') != NULL)
		return (cmd_out);
	path_env = get_env_node_by_key(env, "PATH");
	if (!path_env)
		return (cmd_out);
	path_array = ft_split(((t_env *)path_env->content)->value, ':');
	if (!path_array)
		return (cmd_out);
	free(cmd_out);
	cmd_out = find_exec_path(cmd, path_array);
	free_string_array(path_array);
	return (cmd_out);
}
