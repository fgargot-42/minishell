/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 14:31:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdlib.h>

#define PROMPT CYAN"🐚 "BLUE"minicoquille"CYAN" ⟩ "RESET
#define DEBUG 1

#include "minishell.h"

int	main(void)
{
	char	*line;
	t_cmd	*cmd;
	t_token	*tokens;

	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		tokens = lexer(line);
		if (DEBUG)
			print_tokens(tokens);
		cmd = parser(tokens);
		if (DEBUG)
		{
			print_str_list(cmd->args);
			print_redirs(cmd->redirs);
		}
		//free_tokens(tokens);
		free(line);
	}
	return (0);
}
