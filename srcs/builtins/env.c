/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:55 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/31 21:03:16 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	builtin_env(t_cmd *cmd, t_list **envs)
{
	t_list	*current;
	t_env	*curr_env;

	if (!cmd || !envs || !*envs)
		return (1);
	current = *envs;
	while (current && ((t_env *)current->content)->key)
	{
		curr_env = (t_env *)current->content;
		if (curr_env->value && curr_env->value[0] != '\0')
			printf("%s=%s\n", curr_env->key, curr_env->value);
		current = current->next;
	}
	return (0);
}
