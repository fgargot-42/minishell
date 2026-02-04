/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:56:57 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/22 21:34:58 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest && dest[j])
		j++;
	while (src[i] && (i + j + 1 < size))
	{
		dest[i + j] = src[i];
		i++;
	}
	if (size > 0)
		dest[i + j] = '\0';
	while (src[i])
		i++;
	if (j > size)
		j = size;
	return (i + j);
}
