/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:39 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 22:07:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	builtin_export(t_cmd *cmd, t_list **envs)
{
	(void)cmd;
	(void)envs;
	printf("<EXPORT>\n");
	return (0);
}
