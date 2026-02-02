/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:22:56 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/02 21:59:05 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include <string.h>

static void	del_envlist_content(void *to_delete)
{
	free(((t_env *)to_delete)->key);
	free(((t_env *)to_delete)->value);
}

static void	env_delete(t_list **envs, t_list *to_delete, t_list *prev)
{
	if (!envs)
		return ;
	if (*envs == to_delete)
		*envs = (*envs)->next;
	else
		prev->next = to_delete->next;
	ft_lstdelone(to_delete, &del_envlist_content);
}

int	builtin_unset(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	char	**args;
	t_list	*current;
	t_list	*prev;

	(void)ctx;
	if (!envs)
		return (0);
	args = cmd->args + 1;
	while (*args)
	{
		current = *envs;
		prev = NULL;
		while (current)
		{
			if (!strcmp(((t_env *)current->content)->key, *args))
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
