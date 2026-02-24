/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 02:22:46 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/24 12:41:39 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

static void	init_parts(t_prompt_parts *p, int err)
{
	char	cwd[1024];

	p->errcode = ft_itoa(err);
	p->icon = "🐚 ";
	p->name = "\001minic\002oquille: ";
	getcwd(cwd, sizeof(cwd));
	p->cwd = ft_strdup(cwd);
	p->sep = "⟩ ";
	if (err == 0)
		p->color = BLUE;
	else
		p->color = RED;
}

static void	free_parts(t_prompt_parts *p)
{
	free(p->errcode);
	free(p->cwd);
}

char	*build_prompt(int err)
{
	t_prompt_parts	p;
	char			*prompt;

	init_parts(&p, err);
	if (!p.errcode || !p.cwd)
		return (ft_strdup("🐚 " BLUE "\001minic\002oquille" CYAN " ⟩ " RESET));
	prompt = ft_strjoin("[", p.errcode);
	prompt = join_free(prompt, "] ");
	prompt = join_free(prompt, p.icon);
	prompt = join_free(prompt, p.color);
	prompt = join_free(prompt, p.name);
	prompt = join_free(prompt, p.cwd);
	prompt = join_free(prompt, " ");
	prompt = join_free(prompt, p.color);
	prompt = join_free(prompt, p.sep);
	prompt = join_free(prompt, RESET);
	free_parts(&p);
	return (prompt);
}
