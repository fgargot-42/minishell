/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:48:37 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 14:49:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	printf(CYAN"\n═══════════════════════════ LEXER ═══════════════════════════\n"RESET);
	printf(BLUE"⟩ "RESET);
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
			printf(MAGENTA"WORD"RESET"("CYAN"%s"RESET")", tokens->value);
		else if (tokens->type == TOKEN_PIPE)
			printf(YELLOW"PIPE"RESET);
		else if (tokens->type == TOKEN_REDIR_IN)
			printf(GREEN"REDIR_IN"RESET);
		else if (tokens->type == TOKEN_REDIR_OUT)
			printf(GREEN"REDIR_OUT"RESET);
		else if (tokens->type == TOKEN_APPEND)
			printf(GREEN"APPEND"RESET);
		else if (tokens->type == TOKEN_HEREDOC)
			printf(GREEN"HEREDOC"RESET);
		else if (tokens->type == TOKEN_AND)
			printf(GREEN"&&"RESET);
		else if (tokens->type == TOKEN_OR)
			printf(GREEN"||"RESET);
		else if (tokens->type == TOKEN_LPAREN)
			printf(GREEN"("RESET);
		else if (tokens->type == TOKEN_RPAREN)
			printf(GREEN")"RESET);
		tokens = tokens->next;
		if (tokens && tokens->type != TOKEN_EOF)
			printf(BLUE" → "RESET);
	}
	printf(BLUE" → "RED"EOF\n"RESET);
}
