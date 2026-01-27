/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 19:08:21 by mabarrer         ###   ########.fr       */
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
	t_node	*tree;
	t_token	*tokens;

	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		tokens = lexer(line);
		if (DEBUG)
			print_tokens(tokens);
		tree = parse_tree(tokens);
		print_tree(tree, 0);
		//free_tokens(tokens);
		free(line);
	}
	return (0);
}
