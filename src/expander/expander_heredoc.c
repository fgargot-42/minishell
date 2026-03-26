/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:39:09 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/26 18:44:20 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	is_special_dollar_heredoc(char *input, size_t i)
{
	char	next_char;

	next_char = input[i + 1];
	if (ft_isdigit(next_char))
		return (0);
	return (!ft_isalpha(next_char) && next_char != '_' && next_char != '?');
}

void	expand_var_heredoc(char **input, t_list *envs, t_ctx *ctx)
{
	size_t	i;

	i = 0;
	if (!input || !(*input))
		return ;
	while (i < ft_strlen((*input)))
	{
		if ((*input)[i] != '$' || is_special_dollar_heredoc(*input, i))
			i++;
		else if ((*input)[i + 1] == '?')
			i = replace_errorcode_env(input, i, ctx);
		else if (ft_isdigit((*input)[i + 1]))
			i = replace_numeric_env(input, i, ctx);
		else
			i = expand_regular_var(input, i, envs);
	}
}
