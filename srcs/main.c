/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/06 20:19:09 by mabarrer         ###   ########.fr       */
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


char *handle_input(t_ctx *ctx)
{
	char	*prompt;
	char *line;
	line = NULL;	
	if (isatty(STDIN_FILENO))
	{
		prompt = build_prompt(ctx->error_code);
		line = readline(prompt);
		free(prompt);
		add_history(line);
	}
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}




int	main(int ac, char **av, char **env)
{
	char	*line;
	t_node	*tree;
	t_token	*tokens;
	t_list	*envs;
	t_ctx	ctx;
	
	ctx.error_code = 0;
	(void)ac;
	(void)av;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_handler);
	envs = generate_env(env);
	
	while (1)
	{
		line = handle_input(&ctx);
		if (!line)
			break ;
		if (!*line)
		{
			free(line);
			continue;
		}
		tokens = lexer(line);
		free(line);
		if (DEBUG)
			print_tokens(tokens);
		tree = parse_tree(tokens);
		if (DEBUG)
			print_tree(tree, 0);
		free_tokens(tokens);
		ctx.error_code = exec(tree, &envs, &ctx);
		free_tree(tree);
	}
	
	ft_lstclear(&envs, env_free);
	return (ctx.error_code);
}
