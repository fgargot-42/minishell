/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 22:29:06 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdlib.h>

#define PROMPT CYAN"🐚 "BLUE"\001minicoq\002uille"CYAN" ⟩ "RESET
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
		int errcode = exec(tree);
		printf("LAST ERROR CODE: %d\n", errcode);
		free(line);
	}
	return (0);
}
