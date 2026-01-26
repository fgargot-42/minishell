/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:01:00 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/21 19:41:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	strfindchr(const char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*strim;
	size_t	size;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	while (*s1 && strfindchr(set, *s1))
		s1++;
	size = ft_strlen(s1);
	while (size && strfindchr(set, s1[size - 1]))
		size--;
	strim = malloc(sizeof(char) * (size + 1));
	if (!strim)
		return (NULL);
	i = 0;
	while (i < size)
	{
		strim[i] = s1[i];
		i++;
	}
	strim[i] = '\0';
	return (strim);
}
