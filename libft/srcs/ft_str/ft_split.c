/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:12:44 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/21 19:42:23 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	strcountsub(const char *s, char sep)
{
	int	count;
	int	issep;

	count = 0;
	issep = 1;
	if (!s)
		return (0);
	while (*s)
	{
		if (*s != sep && issep)
		{
			count++;
			issep = 0;
		}
		if (*s == sep)
			issep = 1;
		s++;
	}
	return (count);
}

static void	freeall(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static char	*putnextstr(char **arr, int i, const char *str, char c)
{
	int	countnext;

	while (*str && *str == c)
		str++;
	countnext = 0;
	while (str[countnext] && str[countnext] != c)
		countnext++;
	arr[i] = malloc(sizeof(char) * (countnext + 1));
	if (!arr[i])
	{
		freeall(arr);
		return (NULL);
	}
	ft_strlcpy(arr[i], str, countnext + 1);
	return ((char *)&str[countnext]);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		strcount;
	int		i;

	if (!s)
		return (NULL);
	strcount = strcountsub(s, c);
	strs = malloc(sizeof (char *) * (strcount + 1));
	if (!strs)
		return (NULL);
	strs[strcount] = NULL;
	i = 0;
	while (i < strcount && *s)
	{
		s = putnextstr(strs, i, s, c);
		if (!s)
			return (NULL);
		i++;
	}
	return (strs);
}
