/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:48:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 23:08:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_next_split_noquote(char *str, int *pos)
{
	int		open_squote;
	int		open_dquote;
	int		start;
	char	*res;

	open_squote = 0;
	open_dquote = 0;
	while (str[*pos] && str[*pos] == ' ')
		(*pos)++;
	start = *pos;
	while (str[*pos])
	{
		open_squote = !open_squote && !open_dquote && str[*pos] == '\'';
		open_dquote = !open_squote && !open_dquote && str[*pos] == '\"';
		if (str[*pos] == ' ' && !open_squote && !open_dquote)
			break ;
		(*pos)++;
	}
	res = malloc(sizeof(char) * (*pos - start + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, &str[start], *pos - start + 1);
	pos++;
	return (res);
}

static char	**ft_split_noquote(char *str)
{
	int		count;
	char	**split;
	int		index;
	int		pos;

	index = 0;
	pos = 0;
	count = count_words(str);
	split = malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
	split[count] = NULL;
	while (index < count)
	{
		split[index] = get_next_split_noquote(str, &pos);
		if (!split[index])
		{
			free_string_array(split);
			return (NULL);
		}
		index++;
	}
	return (split);
}

static void	build_new_split(char **split_dst, char **split_src,
	char **split_new, int pos)
{
	int	i;
	int	new_count;

	i = 0;
	new_count = 0;
	while (split_new[new_count])
		new_count++;
	while (i < pos)
	{
		split_dst[i] = split_src[i];
		i++;
	}
	while (i < pos + new_count)
	{
		split_dst[i] = split_new[i - pos];
		i++;
	}
	free(split_src[i - new_count]);
	while (split_src[i - new_count])
	{
		split_dst[i] = split_src[i - new_count + 1];
		i++;
	}
}

int	split_add(char ***split_str, char *new_string, int pos)
{
	char	**new_split;
	char	**split_res;
	int		split_count;
	int		new_count;

	split_count = 0;
	new_count = 0;
	while ((*split_str)[split_count])
		split_count++;
	new_split = ft_split_noquote(new_string);
	if (!new_split)
		return (0);
	new_count = count_words(new_string);
	split_count += new_count;
	split_res = malloc(sizeof(char *) * split_count);
	if (!split_res)
	{
		free_string_array(new_split);
		return (0);
	}
	build_new_split(split_res, *split_str, new_split, pos);
	free(new_split);
	free(*split_str);
	*split_str = split_res;
	return (new_count);
}
