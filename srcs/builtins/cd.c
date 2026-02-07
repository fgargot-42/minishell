/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:24:18 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/07 21:53:26 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	*get_cwd(void)
{
	char	*pwd;
	char	*res;
	int		i;

	i = 128;
	res = NULL;
	while (!res)
	{
		pwd = malloc(i * sizeof(char));
		if (!pwd)
			return (NULL);
		res = getcwd(pwd, i);
		if (!res)
			free(pwd);
		i += 128;
	}
	return (res);
}

static void	update_old_pwd(t_list **envs)
{
	t_list	*pwd_node;

	pwd_node = get_env_node_by_key(*envs, "OLDPWD");
	if (pwd_node)
	{
		free(((t_env *)pwd_node->content)->value);
		((t_env *)pwd_node->content)->value = get_cwd();
	}
}

static void	update_new_pwd(t_list **envs)
{
	t_list	*pwd_node;

	pwd_node = get_env_node_by_key(*envs, "PWD");
	if (pwd_node)
	{
		free(((t_env *)pwd_node->content)->value);
		((t_env *)pwd_node->content)->value = get_cwd();
	}
}

int	builtin_cd(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	t_list	*path;
	char	*str_path;
	int		chdir_status;

	(void)ctx;
	if (!cmd->args[1])
	{
		path = get_env_node_by_key(*envs, "HOME");
		if (!path || !((t_env *)path->content)->value)
		{
			printf("HOME not set");
			return (1);
		}
		else
			chdir_status = chdir(((t_env *)path->content)->value);
	}
	else
	{
		str_path = cmd->args[1];
		update_old_pwd(envs);
		chdir_status = chdir(str_path);
		update_new_pwd(envs);
	}
	return (-chdir_status);
}
