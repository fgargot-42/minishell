/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 21:24:34 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/26 23:05:05 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "../includes/minishell.h"

int	main(void)
{
	char *input;
	while (1)
	{
		input = readline("minishell> ");
		if (!input || !strcmp(input, "exit"))
		{
			printf("exit\n");
			free(input);
			return (0);
		}
		if (!strncmp(input, "echo ", 5))
			builtin_echo(&input[5], 1);
		free(input);
	}
	return (0);
}
