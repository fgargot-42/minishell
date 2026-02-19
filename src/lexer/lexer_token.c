/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 21:16:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 21:26:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_token(t_token_type type, char *value)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	return (tok);
}

static void	skip_whitespace(t_lexer *lexer)
{
	while (current_char(lexer) == ' ' || current_char(lexer) == '\t'
		|| current_char(lexer) == '\n')
		lexer->pos++;
}

static t_token	*get_operator_token(t_lexer *lexer, char c)
{
	if (c == '|')
	{
		lexer->pos++;
		if (current_char(lexer) == '|')
		{
			lexer->pos++;
			return (create_token(TOKEN_OR, "||"));
		}
		return (create_token(TOKEN_PIPE, "|"));
	}
	else
	{
		lexer->pos++;
		if (current_char(lexer) == '&')
		{
			lexer->pos++;
			return (create_token(TOKEN_AND, "&&"));
		}
		fprintf(stderr, "single & error\n");
		return (create_token(TOKEN_EOF, NULL));
	}
}

static t_token	*get_redir_token(t_lexer *lexer, char c)
{
	if (c == '<')
	{
		lexer->pos++;
		if (current_char(lexer) == '<')
		{
			lexer->pos++;
			return (create_token(TOKEN_HEREDOC, "<<"));
		}
		return (create_token(TOKEN_REDIR_IN, "<"));
	}
	else
	{
		lexer->pos++;
		if (current_char(lexer) == '>')
		{
			lexer->pos++;
			return (create_token(TOKEN_APPEND, ">>"));
		}
		return (create_token(TOKEN_REDIR_OUT, ">"));
	}
}

t_token	*get_next_token(t_lexer *lexer)
{
	char	c;

	skip_whitespace(lexer);
	c = current_char(lexer);
	if (c == '\0')
		return (create_token(TOKEN_EOF, NULL));
	else if (c == '|' || c == '&')
		return (get_operator_token(lexer, c));
	else if (c == '<' || c == '>')
		return (get_redir_token(lexer, c));
	else if (c == '(')
	{
		lexer->pos++;
		return (create_token(TOKEN_LPAREN, "("));
	}
	else if (c == ')')
	{
		lexer->pos++;
		return (create_token(TOKEN_RPAREN, ")"));
	}
	else
		return (create_token(TOKEN_WORD, extract_word(lexer)));
}
