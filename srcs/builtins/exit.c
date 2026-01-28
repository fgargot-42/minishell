/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:33:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/28 17:08:29 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

int	builtin_exit(t_cmd *cmd)
{
	int err_code;
	err_code = 0;
	printf("<EXIT>\n");
	printf("exit\n");
	if (cmd->args[1])
		err_code = atoi(cmd->args[1]);
	exit(err_code);
}
