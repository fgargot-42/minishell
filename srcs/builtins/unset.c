/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:22:56 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 19:10:08 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
int	builtin_unset(t_cmd *cmd, t_env *envs)
{
	(void)cmd;
	(void)envs;
	printf("<UNSET>\n");
	return (0);
}
