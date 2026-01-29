/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:39 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/29 20:20:00 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_cmd *cmd, t_list **envs)
{
	(void)cmd;
	printf("<EXPORT>\n");
	if (!envs || !*envs)
		return (0);
	if (!cmd->args[1])
		builtin_export_print(envs);
	return (-1);
}
