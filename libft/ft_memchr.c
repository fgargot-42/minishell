/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:50:15 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/14 16:08:20 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*r;
	size_t			count;

	count = 0;
	r = (unsigned char *)s;
	while (count < n)
	{
		if (*r == (unsigned char)c)
			return ((void *)r);
		count++;
		r++;
	}
	return (0);
}
