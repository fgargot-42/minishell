/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/04 17:13:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	replace_env(char **input, t_env *env, char *key, size_t *pos)
{
	char	*value;
	char	*new_input;
	size_t	value_end_pos;

	value = "";
	if (env && env->value)
		value = env->value;
	new_input = malloc(sizeof(char) * (ft_strlen(*input) + ft_strlen(value)
		- ft_strlen(key)));
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, *pos + 1);
	ft_strlcpy(&new_input[(*pos)], value, ft_strlen(value) + 1);
	value_end_pos = *pos + ft_strlen(value);
	*pos += ft_strlen(key) + 1;
	ft_strlcpy(&new_input[value_end_pos], &((*input)[*pos]),
		ft_strlen(&((*input)[*pos])) + 1);
	free(*input);
	*input = new_input;
}

static void	replace_errorcode_env(char **input, size_t *pos, t_ctx *ctx)
{
	char	*new_input;
	char	*error_str;
	size_t	new_pos;
	
	error_str = ft_itoa(ctx->error_code);
	if (!error_str)
		return ;
	new_input = malloc(sizeof(char) * (ft_strlen(*input)
		+ ft_strlen(error_str) - 1));
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, *pos);
	ft_strlcpy(&new_input[(*pos)], error_str, ft_strlen(error_str) + 1);
	new_pos = *pos + ft_strlen(error_str);
	pos += 2;
	ft_strlcpy(&new_input[new_pos], &((*input)[*pos]),
		ft_strlen(&((*input)[*pos])) + 1);
	free(*input);
	*input = new_input;
}

static t_env	*get_env(t_list *envs, char *key)
{
	t_list	*node;
	t_env	*env;

	env = NULL;
	node = get_env_node_by_key(envs, key); 
	if (node)
		env = (t_env *)node->content;
	return (env);
}

void	expand_var(char **input, t_list *envs, t_ctx *ctx)
{
	size_t	len;
	size_t	i;
	t_env	*env;
	char	*key;

	i = 0;
	while (i < ft_strlen(*input))
	{
		if ((*input)[i] == '$')
		{
			if ((*input)[i + 1] == '?')
				replace_errorcode_env(input, &i, ctx); 
			else
			{
				len = i + 1;
				while (ft_isalnum((*input)[len]) || (*input)[len] == '_')
					len++;
				key = ft_substr(&(*input)[i + 1], 0, len);
				env = get_env(envs, key);
				replace_env(input, env, key, &i);
				free(key);
			}
		}
		i++;
	}
}
