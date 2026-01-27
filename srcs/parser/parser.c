/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:32:10 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 14:38:41 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//debug
#include <string.h>

void	print_str_list(char **str_list)
{
	int	i;

	i = 0;
	printf(CYAN"\n═══════════════════════════ ARGS ════════════════════════════\n"RESET);
	printf(BLUE"⟩ "CYAN"["RESET);
	while (str_list[i])
	{
		printf(MAGENTA"%s"RESET, str_list[i]);
		i++;
		if (str_list[i])
			printf(BLUE", "RESET);
		else
			printf(BLUE", "RESET);
	}
	printf(RED"NULL"CYAN"]\n"RESET);
}

static size_t	count_words(t_token *tokens)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp && tmp->type == TOKEN_WORD)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

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
	while (tokens && tokens->type == TOKEN_WORD)
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
}
