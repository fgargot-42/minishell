/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:33:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 21:46:08 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int	builtin_exit(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	int	err_code;

	(void)envs;
	err_code = ctx->error_code;
	ft_putstr_fd("exit\n", 2);
	if (cmd->args[1])
		err_code = atoi(cmd->args[1]);
	exit(err_code);
}
