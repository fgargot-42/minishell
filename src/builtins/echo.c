/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:37:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/20 17:29:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int	has_option(const char *arg, char option)
{
	if (!arg || arg[0] != '-')
		return (0);
	arg++;
	while (*arg)
	{
		if (*arg == option)
			return (1);
		arg++;
	}
	return (0);
}

static int	is_valid_option_list(const char *arg, const char *option_list)
{
	if (!arg || arg[0] != '-')
		return (0);
	arg++;
	while (*arg)
	{
		if (!ft_strchr(option_list, *arg))
			return (0);
		arg++;
	}
	return (1);
}

int	builtin_echo(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	const char	**av = (const char **)cmd->args;
	int			i;
	int			newline;

	(void)envs;
	(void)ctx;
	newline = 1;
	i = 1;
	while (av[i] && is_valid_option_list(av[i], "n"))
	{
		if (has_option(av[i], 'n'))
			newline = 0;
		i++;
	}
	while (av[i])
	{
		write(1, av[i], ft_strlen(av[i]));
		i++;
		if (av[i])
			write(1, " ", 1);
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
