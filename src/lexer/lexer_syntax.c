/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 20:11:34 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 20:26:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_lexer_errors(t_token *lexer)
{
	t_token	*current;

	current = lexer;
	if (is_operator_token(lexer))
		return (1);
	while (current && current->next)
	{
		if (current->type == TOKEN_RPAREN)
			if (current->next->type == TOKEN_LPAREN
				|| current->next->type == TOKEN_WORD)
				return (1);
		if (is_redir_token(current) || current->type == TOKEN_LPAREN)
			if (current->next->type != TOKEN_WORD)
				return (1);
		if (current->type == TOKEN_WORD && current->next->type == TOKEN_LPAREN)
			return (1);
		if (current->type == TOKEN_AND || current->type == TOKEN_OR
			|| current->type == TOKEN_PIPE)
			if (!is_redir_token(current->next)
				&& current->next->type != TOKEN_WORD
				&& current->next->type != TOKEN_LPAREN)
				return (1);
		current = current->next;
	}
	return (0);
}

int	lexer_has_syntax_error(t_token *lexer, t_ctx *ctx)
{
	if (!lexer || !check_lexer_errors(lexer))
		return (0);
	ctx->error_code = 2;
	free(lexer);
	ft_putstr_fd("Syntax error\n", 2);
	return (1);
}
