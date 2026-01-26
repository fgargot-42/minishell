/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:47:28 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/15 13:55:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/types.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*mem;
	size_t	i;
	size_t	total;

	i = 0;
	total = nmemb * size;
	if (nmemb && total / nmemb != size)
		return (NULL);
	mem = malloc(total);
	if (!mem)
		return (NULL);
	while (i < total)
	{
		mem[i] = '\0';
		i++;
	}
	return ((void *)mem);
}
