/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_replace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:09:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 23:10:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_env(char **input, t_env *env, char *key, size_t *pos)
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

void	replace_errorcode_env(char **input, size_t *pos, t_ctx *ctx)
{
	char	*new_input;
	char	*error_str;
	char	*original;
	size_t	error_len;

	error_str = ft_itoa(ctx->error_code);
	if (!error_str)
		return ;
	original = *input;
	error_len = ft_strlen(error_str);
	new_input = malloc(sizeof(char) * (ft_strlen(original) + error_len - 1));
	if (!new_input)
	{
		free(error_str);
		return ;
	}
	ft_strlcpy(new_input, original, *pos + 1);
	ft_strlcpy(new_input + *pos, error_str, error_len + 1);
	ft_strlcpy(new_input + *pos + error_len, original + *pos + 2,
		ft_strlen(original + *pos + 2) + 1);
	*pos += error_len;
	free(original);
	free(error_str);
	*input = new_input;
}
