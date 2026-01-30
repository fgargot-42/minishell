/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:48:37 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/30 19:48:34 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	fprintf(stderr, CYAN"\n═══════════════════════════ LEXER ═══════════════════════════\n"RESET);
	fprintf(stderr, BLUE"⟩ "RESET);
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
			fprintf(stderr, MAGENTA"WORD"RESET"("CYAN"%s"RESET")", tokens->value);
		else if (tokens->type == TOKEN_PIPE)
			fprintf(stderr, YELLOW"PIPE"RESET);
		else if (tokens->type == TOKEN_REDIR_IN)
			fprintf(stderr, GREEN"REDIR_IN"RESET);
		else if (tokens->type == TOKEN_REDIR_OUT)
			fprintf(stderr, GREEN"REDIR_OUT"RESET);
		else if (tokens->type == TOKEN_APPEND)
			fprintf(stderr, GREEN"APPEND"RESET);
		else if (tokens->type == TOKEN_HEREDOC)
			fprintf(stderr, GREEN"HEREDOC"RESET);
		else if (tokens->type == TOKEN_AND)
			fprintf(stderr, GREEN"&&"RESET);
		else if (tokens->type == TOKEN_OR)
			fprintf(stderr, GREEN"||"RESET);
		else if (tokens->type == TOKEN_LPAREN)
			fprintf(stderr, GREEN"("RESET);
		else if (tokens->type == TOKEN_RPAREN)
			fprintf(stderr, GREEN")"RESET);
		tokens = tokens->next;
		if (tokens && tokens->type != TOKEN_EOF)
			fprintf(stderr, BLUE" → "RESET);
	}
	fprintf(stderr, BLUE" → "RED"EOF\n"RESET);
}
