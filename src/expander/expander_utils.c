/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:42:37 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 23:15:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_dollar(char *input, size_t i, int *open_quotes)
{
	char	next_char;

	if (open_quotes[0])
		return (1);
	next_char = input[i + 1];
	if (open_quotes[1] && (next_char == '\'' || next_char == '\"'))
		return (1);
	return (!ft_isalpha(next_char) && next_char != '_'
		&& next_char != '\'' && next_char != '\"' && next_char != '?');
}

int	count_words(char *s)
{
	int	res;
	int	open_single;
	int	open_double;

	res = *s != 0;
	open_single = 0;
	open_double = 0;
	while (*s)
	{
		if (*s == '\'' && !open_double)
			open_single = !open_single;
		if (*s == '\"' && !open_single)
			open_double = !open_double;
		if (*s == ' ' && !open_single && !open_double)
			res++;
		s++;
	}
	return (res);
}

t_env	*get_env(t_list *envs, char *key)
{
	t_list	*node;
	t_env	*env;

	env = NULL;
	node = get_env_node_by_key(envs, key);
	if (node)
		env = (t_env *)node->content;
	return (env);
}

char	*extract_var_name(char *input, size_t start_pos, size_t *end_pos)
{
	size_t	len;
	char	*var_name;

	len = start_pos;
	while (ft_isalnum(input[len]) || input[len] == '_')
		len++;
	*end_pos = len;
	var_name = ft_substr(&input[start_pos], 0, len - start_pos);
	return (var_name);
}

void	check_open_quotes(char c, int *open_quotes)
{
	if (c == '\'' && !open_quotes[1])
		open_quotes[0] = !open_quotes[0];
	if (c == '\"' && !open_quotes[0])
		open_quotes[1] = !open_quotes[1];
}
