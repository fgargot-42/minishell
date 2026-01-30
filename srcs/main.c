/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:31:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/30 22:31:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#include <unistd.h>
#include <stdlib.h>

//#define PROMPT CYAN"🐚 "BLUE"\001minic\002oquille"CYAN" ⟩ "RESET

#include "minishell.h"
char	*build_prompt(int err)
{
	char	*errcode_str;
	char	*prompt;
	char	cwd[1024];
	
	getcwd(cwd, sizeof(cwd));
	errcode_str = ft_itoa(err);
	if (!errcode_str)
		return (ft_strdup("🐚 "BLUE"minicoquille"CYAN" ⟩ "RESET));
	
	if (err == 0)
	{
		prompt = ft_strjoin(errcode_str, "] 🐚 "BLUE"\001minic\002oquille :");
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, CYAN" ⟩ "RESET);
	}
	else if (err == 127)
	{
		prompt = ft_strjoin(errcode_str, "] 🐚 "RED"\001minic\002oquille :");
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, RED" ⟩ "RESET);
	}
	else
	{
		prompt = ft_strjoin(errcode_str, "] 🐚 "RED"\001minic\002oquille :");
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, RED" ⟩ "RESET);
	}
	
	prompt = ft_strjoin("[", prompt);
	free(errcode_str);
	return (prompt);
}

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
	int errcode;
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
		
		//free_tokens(tokens);
		errcode = exec(tree, &envs);
		free(line);
	}
	return (0);
}
