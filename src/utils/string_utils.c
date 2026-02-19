/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 20:31:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 21:44:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	strlen_noquote(char *str)
{
	int	open_squote;
	int	open_dquote;
	int	i;

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
	res = malloc(sizeof(char) * (res_len + 1));
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

char	*ft_strjoin_chr(char *str1, char *str2, char sep)
{
	char	*res;
	int		reslen;

	reslen = ft_strlen(str1) + ft_strlen(str2) + 1;
	res = malloc(sizeof(char) * (reslen + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, str1, reslen);
	res[ft_strlen(str1)] = sep;
	ft_strlcpy(&res[ft_strlen(str1) + 1], str2, reslen);
	return (res);
}

char	*ft_strjoin_all_chr(char **str_array, char sep)
{
	int		i;
	char	*res;
	char	*old;

	res = ft_strjoin_chr(str_array[0], str_array[1], sep);
	i = 2;
	while (str_array[i])
	{
		old = res;
		res = ft_strjoin_chr(res, str_array[i], sep);
		free(old);
		i++;
	}
	return (res);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
