/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:02:49 by mabarrer          #+#    #+#             */
/*   Updated: 2026/03/23 22:30:24 by fgargot          ###   ########.fr       */
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
	lexer->len = ft_strlen(input);
	return (lexer);
}

char	*extract_word(t_lexer *lexer)
{
	const int	start = lexer->pos;
	int			len;
	char		*word;
	int			open_quotes[2];

	len = 0;
	open_quotes[0] = 0;
	open_quotes[1] = 0;
	while (!(is_special(current_char(lexer)) && !open_quotes[0]
			&& !open_quotes[1]) && current_char(lexer) != '\0'
			&& current_char(lexer) != '\n')
	{
		update_open_quotes(current_char(lexer), open_quotes);
		lexer->pos++;
		len++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, &lexer->input[start], len + 1);
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
