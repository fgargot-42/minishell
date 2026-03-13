/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 21:39:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/14 00:35:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_env	*new_env(char *env_line)
{
	t_env	*new;
	size_t	key_len;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	key_len = 0;
	while (env_line[key_len] && env_line[key_len] != '='
		&& env_line[key_len] != '+')
		key_len++;
	new->key = malloc(sizeof(char) * (key_len + 1));
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	ft_strlcpy(new->key, env_line, key_len + 1);
	new->value = NULL;
	if (env_line[key_len] == '+')
		key_len++;
	new->value = NULL;
	if (env_line[key_len])
		new->value = ft_strdup(&env_line[key_len + 1]);
	return (new);
}

t_list	*generate_env(char **env)
{
	t_list	*head;
	t_env	*new_elem;
	t_list	*env_current;

	head = NULL;
	while (*env)
	{
		new_elem = new_env(*env);
		if (!new_elem)
		{
			ft_lstclear(&head, free);
			return (NULL);
		}
		env_current = ft_lstnew(new_elem);
		if (!env_current)
		{
			free(new_elem);
			ft_lstclear(&head, free);
			return (NULL);
		}
		ft_lstadd_back(&head, env_current);
		env++;
	}
	return (head);
}

static char	*create_env_line(char *key, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	if (!value)
		value = "";
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

char	**reconstruct_envs(t_list *envs)
{
	char	**array;
	t_list	*current;
	int		count;
	int		i;

	count = ft_lstsize(envs);
	current = envs;
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (current)
	{
		array[i] = create_env_line(((t_env *)current->content)->key,
				((t_env *)current->content)->value);
		i++;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}
