/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:39 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/12 17:14:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	check_envname_format(char *str)
{
	if (!str || !*str)
		return (1);
	if (!ft_isalpha(*str) && *str != '_')
		return (1);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
		{
			if (*str == '=' || (*str == '+' && *(str + 1) == '='))
				return (0);
			return (1);
		}
		str++;
	}
	return (0);
}

static int	add_quoted_arg_value(char **arg, char **env)
{
	int		i;
	char	*new_str;
	char	*tmp;

	i = 1;
	while (env[i])
	{
		new_str = ft_strdup(env[i]);
		if (!new_str)
			break ;
		tmp = ft_strjoin(*arg, " ");
		if (!tmp)
		{
			free(new_str);
			break ;
		}
		free(*arg);
		*arg = ft_strjoin(tmp, new_str);
		i++;
		if (!*arg || ft_strchr(new_str, '\"'))
		{
			free(new_str);
			break ;
		}
		free(new_str);
	}
	return (i);
}

static int	add_env_value_append(t_list *node, char **env)
{
	char	*new_str;
	char	*append_str;
	int		i;

	i = 1;
	new_str = ft_strchr(*env, '=');
	if (!new_str)
		return (1);
	new_str++;
	append_str = ft_strjoin(((t_env *)node->content)->value, new_str);
	if (!append_str)
		return (1);
	free(((t_env *)node->content)->value);
	((t_env *)node->content)->value = append_str;
	if (ft_strchr(new_str, '\"'))
		i = add_quoted_arg_value(&((t_env *)node->content)->value, env);
	if (((t_env *)node->content)->value)
		remove_args_quotes(&((t_env *)node->content)->value);
	return (i);
}

static int	add_env_value_replace(t_list *node, char **env)
{
	char	*new_str;;
	int		i;

	i = 1;
	new_str = ft_strchr(*env, '=');
	if (!new_str)
		return (1);
	new_str = ft_strdup(&new_str[1]);
	free(((t_env *)node->content)->value);
	((t_env *)node->content)->value = new_str;
	if (ft_strchr(new_str, '\"'))
		i = add_quoted_arg_value(&((t_env *)node->content)->value, env);
	if (((t_env *)node->content)->value)
		remove_args_quotes(&((t_env *)node->content)->value);
	return (i);
}

static	int add_new_env_value(t_list **env_list, char **env)
{
	char	*new_str;
	t_list	*node;
	int		i;

	i = 1;
	node = new_env(*env);
	ft_lstadd_back(env_list, node);
	new_str = strchr(*env, '=');
	if (new_str && ft_strchr(new_str, '\"'))
		i = add_quoted_arg_value(&((t_env *)node->content)->value, env);
	if (((t_env *)node->content)->value)
		remove_args_quotes(&((t_env *)node->content)->value);
	return (i);
}

static int	add_env(char **env, t_list **env_list)
{
	t_list	*env_node;
	char	*key;
	size_t	key_len;
	int		append;
	int		arg_count;

	arg_count = 1;;
	key_len = ft_strlen(*env);
	key = ft_strchr(*env, '+');
	append = (key != NULL);
	if (!key)
		key = ft_strchr(*env, '=');
	if (key)
		key_len = key - *env;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (1);
	ft_strlcpy(key, *env, key_len + 1);
	env_node = get_env_node_by_key(*env_list, key);
	if (!env_node)
		arg_count = add_new_env_value(env_list, env);
	else if (append)
		arg_count = add_env_value_append(env_node, env);
	else
		arg_count = add_env_value_replace(env_node, env);
	free(key);
	return (arg_count);
}

int	builtin_export(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	char	**args;
	int		i;
	int		status;

	(void)ctx;
	status = 0;
	args = cmd->args;
	if (!envs || !*envs)
		return (0);
	if (!cmd->args[1])
	{
		builtin_export_print(envs);
		return (0);
	}
	i = 1;
	while (*(args + i))
	{
		args += i;
		status = check_envname_format(*args);
		if (status)
		{
			i += 1;
			dprintf(2, "minishell: export: `%s': not a valid identifier\n", *args);
			continue ;
		}
		i = add_env(args, envs);
	}
	return (status);
}
