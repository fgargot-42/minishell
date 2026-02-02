/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:24:41 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/02 21:57:14 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	builtin_pwd(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	char	buf[1024];

	(void)cmd;
	(void)envs;
	(void)ctx;
	if (getcwd(buf, sizeof(buf)))
		printf("%s\n", buf);
	return (0);
}
