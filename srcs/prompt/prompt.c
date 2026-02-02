/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 02:22:46 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/02 22:04:03 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "unistd.h"

static	char *default_prompt(int err)
{
	char	*errcode_str;
	char	*prompt;
	char	cwd[1024];

	getcwd(cwd, sizeof(cwd));
	errcode_str = ft_itoa(err);
	if (!errcode_str)
		return (ft_strdup("🐚 " BLUE "minicoquille" CYAN " ⟩ " RESET));
	if (err == 0)
	{
		prompt = ft_strjoin(errcode_str, "] 🐚 " BLUE "\001minic\002oquille :");
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, CYAN " ⟩ " RESET);
	}
	else if (err == 127)
	{
		prompt = ft_strjoin(errcode_str, "] 🐚 " RED "\001minic\002oquille :");
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, RED " ⟩ " RESET);
	}
	else
	{
		prompt = ft_strjoin(errcode_str, "] 🐚 " RED "\001minic\002oquille :");
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, RED " ⟩ " RESET);
	}
	prompt = ft_strjoin("[", prompt);
	free(errcode_str);
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
