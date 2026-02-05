/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:48:35 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/05 20:01:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*new_str;
	int		nb_quotes;

	i = 0;
	nb_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			nb_quotes++;
		i++;
	}
	new_str = malloc(sizeof(char) * ft_strlen(str) - nb_quotes + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	return (new_str);
}

void	remove_args_quotes(char **args)
{
	int		i;
	char	*new_arg;

	i = 0;
	while (args[i])
	{
		new_arg = remove_quotes(args[i]);
		if (!new_arg)
			return ;
		free(args[i]);
		args[i] = new_arg;
		i++;
	}
}

void	*env_dup_content(void *content)
{
	t_env	*dup;

	dup = malloc(sizeof(t_env));
	if (!dup)
		return (NULL);
	dup->key = ft_strdup(((t_env *)content)->key);
	dup->value = ft_strdup(((t_env *)content)->value);
	return (dup);
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
