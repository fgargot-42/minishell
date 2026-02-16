/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 20:31:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/16 21:35:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	strlen_noquote(char *str)
{
	int		open_squote;
	int		open_dquote;
	int		i;

	open_squote = 0;
	open_dquote = 0;
	i = 0;
	while (*str)
	{
		if (*str == '\'' && !open_dquote)
			open_squote = !open_squote;
		else if (*str == '\"' && !open_squote)
			open_dquote = !open_dquote;
		else
			i++;
		str++;
	}
	return (i);
}

char	*remove_quotes(char *str)
{
	char	*res;
	int		res_len;
	int		open_quotes[2];
	int		i;

	open_quotes[0] = 0;
	open_quotes[1] = 0;
	i = 0;
	res_len = strlen_noquote(str);
	res =  malloc(sizeof(char) * (res_len + 1));
	if (!res)
		return (NULL);
	res[res_len] = '\0';
	while (*str)
	{
		if (*str == '\'' && !open_quotes[1])
			open_quotes[0] = !open_quotes[0];
		else if (*str == '\"' && !open_quotes[0])
			open_quotes[1] = !open_quotes[1];
		else
		{
			res[i] = *str;
			i++;
		}
		str++;
	}
	return (res);
}
