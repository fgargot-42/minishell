/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/18 14:17:01 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "minishell.h"
#include "prompt.h"

atomic_int	g_signal;

static char	*get_readline(t_ctx *ctx)
{
	char	*prompt;
	char	*line;
	int		fdin_tmp;

	signal(SIGINT, sigint_handler);
	fdin_tmp = dup(STDIN_FILENO);
	while (1)
	{
		prompt = build_prompt(ctx->error_code);
		line = readline(prompt);
		free(prompt);
		if (g_signal)
		{
			ctx->error_code = 128 + g_signal;
			g_signal = 0;
			free(line);
			dup2(fdin_tmp, STDIN_FILENO);
			continue ;
		}
		break ;
	}
	close(fdin_tmp);
	signal(SIGINT, sigint_cmd_handler);
	return (line);
}

char	*handle_input(t_ctx *ctx)
{
	char	*line;

	line = NULL;
	if (isatty(STDIN_FILENO))
	{
		line = get_readline(ctx);
		if (line && line[0])
			add_history(line);
	}
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

static void	main_loop(t_list **envs, t_ctx *ctx)
{
	char	*line;
	t_token	*tokens;

	while (!ctx->is_exited)
	{
		g_signal = 0;
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
		ctx->cmd_tree = parse_tree(tokens);
		free_tokens(tokens);
		exec(ctx->cmd_tree, NULL, envs, ctx);
		free_tree(ctx->cmd_tree);
		ctx->cmd_tree = NULL;
	}
}

int	main(int ac, char **av, char **env)
{
	t_list	*envs;
	t_ctx	ctx;

	ctx.error_code = 0;
	ctx.argc = ac;
	ctx.argv = av;
	ctx.is_exited = 0;
	ctx.cmd_tree = NULL;
	signal(SIGQUIT, SIG_IGN);
	envs = generate_env(env);
	main_loop(&envs, &ctx);
	ft_lstclear(&envs, env_free);
	rl_clear_history();
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	return (ctx.error_code);
}
