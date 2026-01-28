/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:33:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/28 22:19:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

int	builtin_exit(t_cmd *cmd, t_list **envs)
{
	(void)envs;
	int err_code;
	err_code = 0;
	printf("<EXIT>\n");
	ft_putstr_fd("exit\n", 2);
	if (cmd->args[1])
		err_code = atoi(cmd->args[1]);
	exit(err_code);
}
