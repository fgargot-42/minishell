/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:42:30 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/27 18:22:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_itoa_hex(int nb)
{
	char	*s;
	char	c;
	size_t	i;

	s = malloc(sizeof(char) * (sizeof(nb) * 2 + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (i < sizeof(nb) * 2)
	{
		c = nb >> (((sizeof(nb) * 2) - 1 - i) * 4) & 0xf;
		if (c >= 10)
			c += 'a' - 10;
		else
			c += '0';
		s[i] = c;
		i++;
	}
	s[i] = 0;
	return (s);
}
