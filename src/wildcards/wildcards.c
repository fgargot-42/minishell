/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 21:02:08 by mabarrer          #+#    #+#             */
/*   Updated: 2026/03/12 18:37:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>

static int	match(char *pattern, char *filename)
{
	if (*pattern == '\0' && *filename == '\0')
		return (1);
	if (*pattern == '*')
	{
		if (match(pattern + 1, filename))
			return (1);
		if (*filename && match(pattern, filename + 1))
			return (1);
		return (0);
	}
	if (*pattern == *filename)
		return (match(pattern + 1, filename + 1));
	return (0);
}

static t_list	*get_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*matches;
	t_list			*new;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (!(entry->d_name[0] == '.' && pattern[0] != '.')
			&& match(pattern, entry->d_name))
		{
			new = ft_lstnew(ft_strdup(entry->d_name));
			ft_lstadd_back(&matches, new);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

static char	**ft_lst_to_array(t_list **list)
{
	int		i;
	int		count;
	char	**result;
	t_list	*node;

	i = 0;
	count = ft_lstsize(*list);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	node = *list;
	while (node)
	{
		result[i] = ft_strdup((char *)(node->content));
		if (!result[i])
		{
			free(result);
			return (NULL);
		}
		i++;
		node = node->next;
	}
	result[i] = NULL;
	return (result);
}

static int	list_strcmp(void *s1, void *s2)
{
	return (ft_strcmp((char *)s1, (char *)s2));
}

char	**expand_wildcards(char *pattern)
{
	t_list	*match_list;
	char	**matches;

	if (!ft_strchr(pattern, '*'))
	{
		matches = malloc(sizeof(char *) * 2);
		if (!matches)
			return (NULL);
		matches[0] = ft_strdup(pattern);
		matches[1] = NULL;
		if (!matches[0])
		{
			free(matches);
			return (NULL);
		}
		return (matches);
	}
	match_list = get_matches(pattern);
	if (ft_lstsize(match_list) >= 2)
		ft_lstsort(&match_list, list_strcmp);
	matches = ft_lst_to_array(&match_list);
	ft_lstclear(&match_list, free);
	return (matches);
}
