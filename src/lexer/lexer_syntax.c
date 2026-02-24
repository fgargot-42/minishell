/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 20:11:34 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/24 01:10:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_lexer_errors(t_token *lexer)
{
	t_token	*current;
	t_token	*next;

	current = lexer;
	if (is_operator_token(lexer))
		return (1);
	while (current && current->next)
	{
		next = current->next;
		if (current->type == TOKEN_RPAREN)
			if (next->type == TOKEN_LPAREN || next->type == TOKEN_WORD)
				return (1);
		if (is_redir_token(current) && next->type != TOKEN_WORD)
			return (1);
		if (current->type == TOKEN_WORD && next->type == TOKEN_LPAREN)
			return (1);
		if (is_operator_token(current) || current->type == TOKEN_LPAREN)
			if (!is_redir_token(next) && next->type != TOKEN_WORD
				&& next->type != TOKEN_LPAREN)
				return (1);
		current = current->next;
	}
	return (0);
}

static int	check_parenthesis_pairs(t_token *lexer)
{
	int	lparen;
	int	rparen;

	lparen = 0;
	rparen = 0;
	while (lexer)
	{
		if (lexer->type == TOKEN_LPAREN)
			lparen++;
		if (lexer->type == TOKEN_RPAREN)
			rparen++;
		lexer = lexer->next;
	}
	return (lparen == rparen);
}

int	lexer_has_syntax_error(t_token *lexer, t_ctx *ctx)
{
	if (!lexer || (!check_lexer_errors(lexer)
			&& check_parenthesis_pairs(lexer)))
		return (0);
	ctx->error_code = 2;
	free_tokens(lexer);
	ft_putstr_fd("minishell: syntax error\n", 2);
	return (1);
}
