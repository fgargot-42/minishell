/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:32:10 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/29 20:07:33 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// debug
#include <string.h>

static size_t	count_words(t_token *tokens)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		if (is_redirection(tmp->type))
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (count);
}
/*
t_cmd	*parser(t_token *tokens)
{
	t_cmd			*cmd;
	int				i;
	const size_t	count = count_words(tokens);

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->args = malloc(sizeof(char *) * (count + 1));
	cmd->redirs = NULL;
	// parsing args
	i = 0;
	while (tokens  == TOKEN_WORD)
	{
		cmd->args[i] = strdup(tokens->value);
		i++;
		tokens = tokens->next;
	}
	cmd->args[i] = NULL;
	// parsings redirs
	while (tokens && is_redirection(tokens->type))
		add_redirection(cmd, &tokens);
	return (cmd);
}*/
