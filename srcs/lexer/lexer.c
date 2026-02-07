/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:02:49 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/07 20:43:05 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

static t_lexer	*init_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
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
	while (current_char(lexer) == ' ' || current_char(lexer) == '\t' || current_char(lexer) == '\n')
		lexer->pos++;
}

static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t'
		|| c == '\0' || c == '&' || c == '(' || c == ')');
}

static char *extract_quoted_word(t_lexer *lexer, char quote_char, t_quote_type *quote_type)
{
	int start;
	int len;
	char *word;
	lexer->pos++;
	start = lexer->pos;
	len = 0;

	while (lexer->pos < lexer->len && current_char(lexer) != quote_char)
	{
		if (quote_char == '"' && current_char(lexer) == '\\')
		{
			lexer->pos++;
			if (lexer->pos < lexer->len)
				lexer->pos++;
			len += 2;
			continue ;
		}
		lexer->pos++;
		len++;
	}
	if (current_char(lexer) != quote_char)
	{
		fprintf(stderr, "Error: unclosed quote\n");
		return (NULL);
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	strncpy(word, &lexer->input[start], len);
	word[len] = '\0';
	lexer->pos++;
	
	if (quote_char == '\'')
		*quote_type = QUOTE_SINGLE;
	else
		*quote_type = QUOTE_DOUBLE;
	return (word);
}
static char	*extract_word(t_lexer *lexer)
{
	const int	start = lexer->pos;
	int			len;
	char		*word;

	len = 0;
	while (!is_special(current_char(lexer)) && current_char(lexer) != '\n')
	{
		lexer->pos++;
		len++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	strncpy(word, &lexer->input[start], len);
	word[len] = '\0';
	return (word);
}

t_token	*create_token(t_token_type type, char *value, t_quote_type quote)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	tok->quote = quote;
	return (tok);
}

t_token	*get_next_token(t_lexer *lexer)
{
	char	c;
	t_quote_type quote;
	skip_whitespace(lexer);
	c = current_char(lexer);
	quote = QUOTE_NONE;
	if (c == '\0')
		return (create_token(TOKEN_EOF, NULL, QUOTE_NONE));
	else if (c == '|')
	{
		lexer->pos++;
		if (current_char(lexer) == '|')
		{
			lexer->pos++;
			return (create_token(TOKEN_OR, "||", QUOTE_NONE));
		}
		return (create_token(TOKEN_PIPE, "|", QUOTE_NONE));
	}
	else if (c == '<')
	{
		lexer->pos++;
		if (current_char(lexer) == '<')
		{
			lexer->pos++;
			return (create_token(TOKEN_HEREDOC, "<<", QUOTE_NONE));
		}
		return (create_token(TOKEN_REDIR_IN, "<", QUOTE_NONE));
	}
	else if (c == '>')
	{
		lexer->pos++;
		if (current_char(lexer) == '>')
		{
			lexer->pos++;
			return (create_token(TOKEN_APPEND, ">>", QUOTE_NONE));
		}
		return (create_token(TOKEN_REDIR_OUT, ">", QUOTE_NONE));
	}
	else if (c == '&')
	{
		lexer->pos++;
		if (current_char(lexer) == '&')
		{
			lexer->pos++;
			return (create_token(TOKEN_AND, "&&", QUOTE_NONE));
		}
		fprintf(stderr, "single & error\n");
		return (create_token(TOKEN_EOF, NULL, QUOTE_NONE)); // temporaire
	}
	else if (c == '(')
	{
		lexer->pos++;
		return (create_token(TOKEN_LPAREN, "(", QUOTE_NONE));
	}
	else if (c == ')')
	{
		lexer->pos++;
		return (create_token(TOKEN_RPAREN, ")", QUOTE_NONE));
	}
	else if (c == '"')
		return (create_token(TOKEN_WORD, extract_quoted_word(lexer, '"', &quote), quote));
	else if (c == '\'')
		return (create_token(TOKEN_WORD, extract_quoted_word(lexer, '\'', &quote), quote));
	else
		return (create_token(TOKEN_WORD, extract_word(lexer), quote));
}

static int	is_redir_token(t_token *token)
{
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_HEREDOC || token->type == TOKEN_APPEND);
}

int	check_lexer_errors(t_token *lexer)
{
	t_token	*current;

	current = lexer;
	while (current && current->next)
	{
		if (current->type == TOKEN_RPAREN)
			if (current->next->type == TOKEN_LPAREN || current->next->type == TOKEN_WORD)
				return (1);
		if (is_redir_token(current) || current->type == TOKEN_LPAREN)
			if (current->next->type != TOKEN_WORD)
				return (1);
		if (current->type == TOKEN_WORD && current->next->type == TOKEN_LPAREN)
			return (1);
		if (current->type == TOKEN_AND || current->type == TOKEN_OR
			|| current->type == TOKEN_PIPE)
			if (!is_redir_token(current->next) && current->next->type != TOKEN_WORD
				&& current->next->type != TOKEN_LPAREN)
				return (1);
		current = current->next;
	}
	return (0);
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
