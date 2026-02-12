/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 02:22:46 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/12 17:16:49 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "unistd.h"
static char *join_free(char *s1, char *s2, int free_s1, int free_s2)
{
    char *result;
    
    result = ft_strjoin(s1, s2);
    if (free_s1)
        free(s1);
    if (free_s2)
        free(s2);
    return (result);
}

static void init_parts(t_prompt_parts *p, int err)
{
    char cwd[1024];
    
    p->errcode = ft_itoa(err);
    p->icon = "🐚 ";
    p->name = "\001minic\002oquille";
    getcwd(cwd, sizeof(cwd));
    p->cwd = ft_strdup(cwd);
    p->sep = "⟩";
    if (err == 0)
        p->color = BLUE;
    else
        p->color = RED;
}

static void free_parts(t_prompt_parts *p)
{
    free(p->errcode);
    free(p->cwd);
}

static char *default_prompt(int err)
{
    t_prompt_parts p;
    char *prompt;

    init_parts(&p, err);
    if (!p.errcode || !p.cwd)
        return (ft_strdup("🐚 " BLUE "minicoquille" CYAN " ⟩ " RESET));
    
    prompt = ft_strjoin("[", p.errcode);
    prompt = join_free(prompt, "] ", 1, 0);
    prompt = join_free(prompt, p.icon, 1, 0);
    prompt = join_free(prompt, p.color, 1, 0);
    prompt = join_free(prompt, p.name, 1, 0);
    prompt = join_free(prompt, ": ", 1, 0);
    prompt = join_free(prompt, p.cwd, 1, 0);
    prompt = join_free(prompt, " ", 1, 0);
    prompt = join_free(prompt, p.color, 1, 0);
    prompt = join_free(prompt, p.sep, 1, 0);
    prompt = join_free(prompt, RESET, 1, 0);
    
    free_parts(&p);
    return (prompt);
}
static char *debug_prompt(int err)
{
	(void)err;
	return strdup("minishellgdb#");
}
char	*build_prompt(int err)
{
	if (DEBUG)
		return (debug_prompt(err));
	else
		return (default_prompt(err));
}
