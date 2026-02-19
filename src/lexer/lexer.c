/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:02:49 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/19 21:39:27 by fgargot          ###   ########.fr       */
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

char	*extract_word(t_lexer *lexer)
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

static void	token_add_back(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

t_token	*lexer(char *input)
{
	t_lexer	*l;
	t_token	*head;
	t_token	*new_token;

	l = init_lexer(input);
	head = NULL;
	new_token = NULL;
	while (1)
	{
		new_token = get_next_token(l);
		token_add_back(&head, new_token);
		if (new_token->type == TOKEN_EOF)
			break ;
	}
	free(l);
	return (head);
}
