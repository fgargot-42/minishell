/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:24:18 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/20 19:31:18 by fgargot          ###   ########.fr       */
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

static int	cd_home(t_list **envs)
{
	t_list	*path;

	path = get_env_node_by_key(*envs, "HOME");
	if (!path || !((t_env *)path->content)->value)
	{
		ft_putstr_fd("HOME not set\n", 2);
		return (1);
	}
	return (-chdir(((t_env *)path->content)->value));
}

int	builtin_cd(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	(void)ctx;
	if (cmd->args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (!cmd->args[1])
		return (cd_home(envs));
	update_old_pwd(envs);
	if (chdir(cmd->args[1]))
		return (1);
	update_new_pwd(envs);
	return (0);
}
