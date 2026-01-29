/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:39 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/29 18:23:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*env_dup_content(void *content)
{
	t_env	*dup;

	dup = malloc(sizeof(t_env));
	if (!dup)
		return (NULL);
	dup->key = ft_strdup(((t_env*)content)->key);
	dup->value = ft_strdup(((t_env*)content)->value);
	return (dup);
}

void	env_free(void *content)
{
	free(((t_env *)content)->key);
	free(((t_env *)content)->value);
	free(content);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (0);
}

int	env_cmp(void *a, void *b)
{
	return (ft_strcmp(((t_env *)a)->key, ((t_env *)b)->key));
}

int	builtin_export(t_cmd *cmd, t_list **envs)
{
	t_list	*env_dup;
	t_list	*current;

	if (cmd->args[1])
		return (1);
	if (!envs || !*envs)
		return (0);
	env_dup = ft_lstmap(*envs, &env_dup_content, &env_free);
	ft_lstsort(&env_dup, &env_cmp);
	(void)cmd;
	current = env_dup;
	printf("<EXPORT>\n");
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", ((t_env *)current->content)->key,
			((t_env *)current->content)->value);
		current = current->next;
	}
	ft_lstclear(&env_dup, &env_free);
	return (0);
}
