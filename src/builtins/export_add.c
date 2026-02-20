/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 16:54:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/20 17:10:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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
	char	*new_str;
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

static int	add_new_env_value(t_list **env_list, char **env)
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

static char	*env_get_key(char *env)
{
	char	*key;
	size_t	key_len;

	key_len = ft_strlen(env);
	key = ft_strchr(env, '+');
	if (!key)
		key = ft_strchr(env, '=');
	if (key)
		key_len = key - env;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, env, key_len + 1);
	return (key);
}

int	add_env(char **env, t_list **env_list)
{
	t_list	*env_node;
	char	*key;
	char	*append;
	char	*equal;
	int		arg_count;

	arg_count = 1;
	equal = ft_strchr(*env, '=');
	append = ft_strchr(*env, '+');
	key = env_get_key(*env);
	if (!key)
		return (1);
	env_node = get_env_node_by_key(*env_list, key);
	free(key);
	if (!env_node)
		arg_count = add_new_env_value(env_list, env);
	else if (append != NULL && append < equal)
		arg_count = add_env_value_append(env_node, env);
	else
		arg_count = add_env_value_replace(env_node, env);
	return (arg_count);
}
