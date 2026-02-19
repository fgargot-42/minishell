/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:02:49 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/19 20:18:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

static t_lexer	*init_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = strlen(input);
	return (lexer);
}

static char	current_char(t_lexer *lexer)
{
	if (lexer->pos >= lexer->len)
		return ('\0');
	else
		return (lexer->input[lexer->pos]);
}

static void	skip_whitespace(t_lexer *lexer)
{
	while (current_char(lexer) == ' ' || current_char(lexer) == '\t'
		|| current_char(lexer) == '\n')
		lexer->pos++;
}

static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t'
		|| c == '\0' || c == '&' || c == '(' || c == ')');
}

static char	*extract_word(t_lexer *lexer)
{
	const int	start = lexer->pos;
	int			len;
	char		*word;
	int			open_single;
	int			open_double;

	len = 0;
	open_single = 0;
	open_double = 0;
	while (!(is_special(current_char(lexer)) && !open_single && !open_double)
		&& current_char(lexer) != '\n')
	{
		if (!open_double && current_char(lexer) == '\'')
			open_single = !open_single;
		if (!open_single && current_char(lexer) == '\"')
			open_double = !open_double;
		lexer->pos++;
		len++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	strncpy(word, &lexer->input[start], len);
	word[len] = '\0';
	return (word);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	return (tok);
}

t_token	*get_next_token(t_lexer *lexer)
{
	char	c;

	skip_whitespace(lexer);
	c = current_char(lexer);
	if (c == '\0')
		return (create_token(TOKEN_EOF, NULL));
	else if (c == '|')
	{
		lexer->pos++;
		if (current_char(lexer) == '|')
		{
			lexer->pos++;
			return (create_token(TOKEN_OR, "||"));
		}
		return (create_token(TOKEN_PIPE, "|"));
	}
	else if (c == '<')
	{
		lexer->pos++;
		if (current_char(lexer) == '<')
		{
			lexer->pos++;
			return (create_token(TOKEN_HEREDOC, "<<"));
		}
		return (create_token(TOKEN_REDIR_IN, "<"));
	}
	else if (c == '>')
	{
		lexer->pos++;
		if (current_char(lexer) == '>')
		{
			lexer->pos++;
			return (create_token(TOKEN_APPEND, ">>"));
		}
		return (create_token(TOKEN_REDIR_OUT, ">"));
	}
	else if (c == '&')
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

t_token	*lexer(char *input)
{
	t_lexer	*l;
	t_token	*head;
	t_token	*current;
	t_token	*new_token;

	l = init_lexer(input);
	head = NULL;
	current = NULL;
	new_token = NULL;
	while (1)
	{
		new_token = get_next_token(l);
		if (!head)
		{
			head = new_token;
			current = head;
		}
		else
		{
			current->next = new_token;
			current = new_token;
		}
		if (new_token->type == TOKEN_EOF)
			break ;
	}
	free(l);
	return (head);
}
