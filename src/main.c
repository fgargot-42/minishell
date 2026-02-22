/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/22 00:20:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#define PROMPT CYAN"🐚 "BLUE"\001minic\002oquille"CYAN" ⟩ "RESET

#include "minishell.h"
#include "prompt.h"

char	*handle_input(t_ctx *ctx)
{
	char	*prompt;
	char	*line;

	line = NULL;
	if (isatty(STDIN_FILENO))
	{
		prompt = build_prompt(ctx->error_code);
		line = readline(prompt);
		free(prompt);
		if (line)
			add_history(line);
	}
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

static void	main_loop(t_list *envs, t_ctx *ctx)
{
	char	*line;
	t_node	*tree;
	t_token	*tokens;

	while (1)
	{
		line = handle_input(ctx);
		if (!line)
			break ;
		if (!*line)
		{
			free(line);
			continue ;
		}
		tokens = lexer(line);
		free(line);
		if (lexer_has_syntax_error(tokens, ctx))
			continue ;
		tree = parse_tree(tokens);
		free_tokens(tokens);
		exec(tree, &envs, ctx);
		free_tree(tree);
	}
}

int	main(int ac, char **av, char **env)
{
	t_list	*envs;
	t_ctx	ctx;

	ctx.error_code = 0;
	(void)ac;
	(void)av;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_handler);
	envs = generate_env(env);
	main_loop(envs, &ctx);
	ft_lstclear(&envs, env_free);
	rl_clear_history();
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	return (ctx.error_code);
}
