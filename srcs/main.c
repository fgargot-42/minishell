/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 19:20:25 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//#define PROMPT CYAN"🐚 "BLUE"\001minic\002oquille"CYAN" ⟩ "RESET

#include "minishell.h"
#include "prompt.h"

static void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_node	*tree;
	t_token	*tokens;
	t_list	*envs;
	char	*p;
	int		errcode;

	errcode = 0;
	(void)ac;
	(void)av;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_handler);
	envs = generate_env(env);
	while (1)
	{
		p = build_prompt(errcode);
		line = readline(p);
		if (!line)
			break ;
		if (line)
			add_history(line);
		tokens = lexer(line);
		if (DEBUG)
			print_tokens(tokens);
		tree = parse_tree(tokens);
		if (DEBUG)
			print_tree(tree, 0);
		// free_tokens(tokens);
		errcode = exec(tree, &envs);
		free(line);
	}
	return (0);
}
