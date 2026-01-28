/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:55 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 17:41:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	builtin_env(t_cmd *cmd)
{
	t_env	current;

	printf("<ENV>\n");
	if (!cmd && !cmd->env)
		return (1);
	current = cmd->env;
	while (current && current->key)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
