/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 02:22:46 by mabarrer          #+#    #+#             */
/*   Updated: 2026/03/18 14:12:48 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "prompt.h"
#include "unistd.h"

static char	*join_free(char *s1, char *s2)
{
	char	*result;

	result = NULL;
	if (s1 && s2)
		result = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	return (result);
}

static void	init_parts(t_prompt_parts *p, int exit_code)
{
	char	cwd[1024];

	p->exit_code = ft_itoa(exit_code);
	p->icon = "\001🐚\002 ";
	p->name = "minicoquille: ";
	getcwd(cwd, sizeof(cwd));
	p->cwd = ft_strdup(cwd);
	p->sep = "⟩ ";
	p->color = BLUE;
	if (exit_code)
		p->color = RED;
}

char	*build_prompt(int exit_code)
{
	t_prompt_parts	p;
	char			*prompt;

	init_parts(&p, exit_code);
	if (!p.cwd)
		return (ft_strdup("\001🐚\002 " BLUE "minicoquille: " CYAN "⟩ " RESET));
	prompt = ft_strjoin(p.color, "[");
	prompt = join_free(prompt, p.exit_code);
	prompt = join_free(prompt, "] ");
	prompt = join_free(prompt, p.icon);
	prompt = join_free(prompt, p.name);
	prompt = join_free(prompt, p.cwd);
	prompt = join_free(prompt, " ");
	prompt = join_free(prompt, p.sep);
	prompt = join_free(prompt, RESET);
	free(p.cwd);
	free(p.exit_code);
	return (prompt);
}
