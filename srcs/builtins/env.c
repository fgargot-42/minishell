/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:55 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 19:10:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	builtin_env(t_cmd *cmd, t_env *envs)
{
	t_env	*current;

	printf("<ENV>\n");
	if (!cmd && !envs)
		return (1);
	current = envs;
	while (current && current->key)
	{
		if (current->value && current->value[0] != '\0')
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
