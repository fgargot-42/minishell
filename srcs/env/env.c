/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 21:39:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/05 17:33:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_list	*new_env(char *env_line)
{
	t_env	*new;
	size_t	key_len;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	key_len = 0;
	while (env_line[key_len] && env_line[key_len] != '=')
		key_len++;
	new->key = malloc(sizeof(char) * (key_len + 1));
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	ft_strlcpy(new->key, env_line, key_len + 1);
	new->value = NULL;
	if (env_line[key_len])
	{
		new->value = malloc(sizeof(char) * (ft_strlen(env_line) - key_len));
		if (!new->value)
		{
			free(new->key);
			free(new);
			return (NULL);
		}
		ft_strlcpy(new->value, &env_line[key_len + 1], ft_strlen(&env_line[key_len]));
	}
	return (ft_lstnew(new));
}

t_list	*generate_env(char **env)
{
	t_list	*head;

	head = NULL;
	while (*env)
	{
		ft_lstadd_back(&head, new_env(*env));
		env++;
	}
	return (head);
}

void	print_env_export(t_list *env)
{
	t_list	*current;

	current = env;
	while (current != NULL)
	{
		printf("%s=%s\n", ((t_env *)current->content)->key,
			((t_env *)current->content)->value);
		current = current->next;
	}
}

// key value -> KEY=VALUE char*
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

const char	**reconstruct_envs(t_list *envs)
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
	return ((const char **)array);
}
