/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:22:56 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 20:47:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include <string.h>

static void	env_delete(t_env **envs, t_env *to_delete, t_env *prev)
{
	if (!envs)
		return ;
	if (*envs == to_delete)
		*envs = (*envs)->next;
	else
		prev->next = to_delete->next;
	free(to_delete->key);
	free(to_delete->value);
	free(to_delete);
}

int	builtin_unset(t_cmd *cmd, t_env **envs)
{
	char	**args;
	t_env	*current;
	t_env	*prev;

	printf("<UNSET>\n");
	if (!envs)
		return (0);
	args = cmd->args + 1;
	while (*args)
	{
		current = *envs;
		prev = NULL;
		while (current)
		{
			if (!strcmp(current->key, *args))
			{
				env_delete(envs, current, prev);
				break ;
			}
			prev = current;
			current = current->next;
		}
		args++;
	}
	return (0);
}
