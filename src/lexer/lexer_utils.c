/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:48:37 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/20 22:15:46 by fgargot          ###   ########.fr       */
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
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_HEREDOC || token->type == TOKEN_APPEND);
}

int	is_operator_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_AND || token->type == TOKEN_OR
		|| token->type == TOKEN_PIPE);
}

char	current_char(t_lexer *lexer)
{
	if (lexer->pos >= lexer->len)
		return ('\0');
	else
		return (lexer->input[lexer->pos]);
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t'
		|| c == '\0' || c == '&' || c == '(' || c == ')');
}
