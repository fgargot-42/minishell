/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:10:11 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/21 19:43:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		count;
	char	*map;

	if (!s || !f)
		return (NULL);
	count = 0;
	while (s[count])
		count++;
	map = malloc(sizeof(char) * (count + 1));
	if (!map)
		return (NULL);
	map[count] = '\0';
	count = 0;
	while (s[count])
	{
		map[count] = (*f)(count, s[count]);
		count++;
	}
	return (map);
}
