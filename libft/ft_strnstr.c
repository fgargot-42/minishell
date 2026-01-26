/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:38:40 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/21 19:34:26 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	count;
	size_t	sub;

	if (*little == '\0')
		return ((char *)big);
	count = 0;
	while (*big)
	{
		if (*big == *little)
		{
			sub = 1;
			while (little[sub] && big[sub] == little[sub])
				sub++;
			if (little[sub] == '\0' && count + sub <= len)
				return ((char *)big);
		}
		big++;
		count++;
	}
	return (0);
}
