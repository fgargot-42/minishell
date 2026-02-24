/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_replace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:09:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/23 18:39:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_env(char *input, char *rep, size_t pos, size_t env_len)
{
	char	*new_input;
	char	*input_end;

	new_input = malloc(sizeof(char) * (ft_strlen(input) + ft_strlen(rep)
				- env_len));
	if (!new_input)
		return (NULL);
	ft_strlcpy(new_input, input, pos + 1);
	ft_strlcpy(&new_input[pos], rep, ft_strlen(rep) + 1);
	input_end = &(input[pos + env_len + 1]);
	pos += ft_strlen(rep);
	ft_strlcpy(&new_input[pos], input_end, ft_strlen(input_end) + 1);
	return (new_input);
}

size_t	replace_regular_env(char **input, t_env *env, char *key, size_t pos)
{
	char	*value;
	char	*new_input;

	value = "";
	if (env && env->value)
		value = env->value;
	new_input = replace_env(*input, value, pos, ft_strlen(key));
	if (!new_input)
		return (pos + 1);
	free(*input);
	*input = new_input;
	return (pos + ft_strlen(value));
}

size_t	replace_numeric_env(char **input, size_t pos, t_ctx *ctx)
{
	char	*value;
	size_t	value_len;
	char	*new_input;
	int		arg_nbr;

	arg_nbr = (*input)[1] - '0';
	if (arg_nbr >= ctx->argc)
		value = ft_strdup("");
	else
		value = ft_strdup(ctx->argv[arg_nbr]);
	if (!value)
		return (pos + 1);
	new_input = replace_env(*input, value, pos, 1);
	value_len = ft_strlen(value);
	free(value);
	if (!new_input)
		return (pos + 1);
	free(*input);
	*input = new_input;
	return (pos + value_len);
}

size_t	replace_errorcode_env(char **input, size_t pos, t_ctx *ctx)
{
	char	*new_input;
	char	*error_str;
	size_t	error_len;

	error_str = ft_itoa(ctx->error_code);
	if (!error_str)
		return (pos + 1);
	new_input = replace_env(*input, error_str, pos, 1);
	error_len = ft_strlen(error_str);
	free(error_str);
	if (!new_input)
		return (pos + 1);
	free(*input);
	*input = new_input;
	return (pos + error_len);
}
