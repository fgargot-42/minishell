/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/28 18:30:49 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdlib.h>

#define PROMPT CYAN"🐚 "BLUE"\001minic\002oquille"CYAN" ⟩ "RESET
#define DEBUG 1

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_node	*tree;
	t_token	*tokens;
	(void)ac;
	(void)av;
	t_env	*envs;

	envs = generate_env(env);
	print_env_export(envs);
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
		int errcode = exec(tree, envs);
		printf("LAST ERROR CODE: %d\n", errcode);
		free(line);
	}
	return (0);
}
