/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:39 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/02 21:56:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_envname_format(char *str)
{
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
		{
			if (*str == '=' || (*str == '+' && *(str + 1) == '='))
				return (1);
			return (0);
		}
		str++;
	}
	return (1);
}

static void	add_env_value_append(t_list *node, char *env)
{
	char	*new_str;
	char	*append_str;

	new_str = ft_strchr(env, '=');
	if (!new_str)
		return ;
	new_str++;
	append_str = ft_strjoin(((t_env *)node->content)->value, new_str);
	if (!append_str)
		return ;
	free(((t_env *)node->content)->value);
	((t_env *)node->content)->value = append_str;
}

static void	add_env_value_replace(t_list *node, char *env)
{
	char	*new_str;

	new_str = ft_strchr(env, '=');
	if (!new_str)
		return ;
	new_str = ft_strdup(&new_str[1]);
	free(((t_env *)node->content)->value);
	((t_env *)node->content)->value = new_str;
}

static void	add_env(char *env, t_list **env_list)
{
	t_list	*env_node;
	char	*key;
	size_t	key_len;
	int		append;

	key_len = ft_strlen(env);
	key = ft_strchr(env, '+');
	append = (key != NULL);
	if (!key)
		key = ft_strchr(env, '=');
	if (key)
		key_len = key - env;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return ;
	ft_strlcpy(key, env, key_len + 1);
	env_node = get_env_node_by_key(*env_list, key);
	if (!env_node)
		ft_lstadd_back(env_list, new_env(env));
	else if (append)
		add_env_value_append(env_node, env);
	else
		add_env_value_replace(env_node, env);
	free(key);
}

int	builtin_export(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	char	**args;
	int		exit_status;

	(void)ctx;
	exit_status = 0;
	args = cmd->args;
	if (!envs || !*envs)
		return (0);
	if (!cmd->args[1])
	{
		builtin_export_print(envs);
		return (0);
	}
	while (*(args + 1))
	{
		args++;
		if (!check_envname_format(*args))
		{
			exit_status = 1;
			continue ;
		}
		add_env(*args, envs);
	}
	return (exit_status);
}
