/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:37:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/30 17:46:59 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include <string.h>

static int	has_option(const char *arg, char option)
{
	if (!arg ||  arg[0] != '-')
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
	if (!arg ||  arg[0] != '-')
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

int	builtin_echo(t_cmd *cmd, t_list **envs)
{
	(void)envs;
	const char **av = (const char **)cmd->args;
	int i;
	char	newline;

	printf("<ECHO>\n");
	newline = '\n';
	i = 1;
	if (is_valid_option_list(av[1], "npc"))
	{
		i++;
		if (has_option(av[1], 'n'))
			newline = 0;
	}
	while (av[i])
	{
		printf("%s", av[i]);
		i++;
		if (av[i])
			printf("%c", ' ');
	}
	printf("%c", newline);
	return (0);
}
