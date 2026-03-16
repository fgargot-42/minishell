/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 00:17:56 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/16 20:19:54 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "prompt.h"
#include <readline/readline.h>
#include <unistd.h>
#include <stdatomic.h>

extern atomic_int	g_signal;

void	sigint_handler(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(STDIN_FILENO);
}

void	sigint_cmd_handler(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

void	sigpipe_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("Broken pipe\n", 2);
}
