/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:37:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 21:57:28 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include <string.h>
int	builtin_echo(t_cmd *cmd)
{
	const char **av = (const char **)cmd->args;
	int i;

	printf("<ECHO>\n");
	if (strcmp(av[1], "-n") == 0)
	{
		i = 2;
		while (av[i])
		{
			printf("%s ", av[i]);
			i++;	
		}
	}
	else
	{
		i = 1;
		while (av[i])
		{
			printf("%s", av[i]);
			i++;
		}
		printf("\n");
	}
	
	return (0);
}
