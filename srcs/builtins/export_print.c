/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:48:35 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/29 20:19:26 by fgargot          ###   ########.fr       */
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

void	builtin_export_print(t_list **envs)
{
	t_list	*env_dup;
	t_list	*current;

	env_dup = ft_lstmap(*envs, &env_dup_content, &env_free);
	ft_lstsort(&env_dup, &env_cmp);
	current = env_dup;
	while (current)
	{
		printf("declare -x %s", ((t_env *)current->content)->key);
		if (((t_env *)current->content)->value)
			printf("=\"%s\"", ((t_env *)current->content)->value);
		printf("\n");	
		current = current->next;
	}
	ft_lstclear(&env_dup, &env_free);
}
