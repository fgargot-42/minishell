/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:48:37 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 20:18:54 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->type == TOKEN_WORD && tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

int	is_redir_token(t_token *token)
{
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_HEREDOC || token->type == TOKEN_APPEND);
}

int	is_operator_token(t_token *token)
{
	return (token->type == TOKEN_AND || token->type == TOKEN_OR
		|| token->type == TOKEN_PIPE);
}
