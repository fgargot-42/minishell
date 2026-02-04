/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:01:22 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/16 14:01:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	size_itoa(int n)
{
	int	size;

	size = 1;
	if (n == 0)
		return (2);
	if (n < 0)
		size++;
	while (n)
	{
		size++;
		n /= 10;
	}
	return (size);
}

static void	write_itoa(int n, char *str, int size)
{
	char	c;

	if (n < 0)
		str[0] = '-';
	c = n % 10 + '0';
	if (n < 0)
		c = -(n % 10) + '0';
	if (size > 2)
	{
		str[size - 2] = c;
		write_itoa(n / 10, str, size - 1);
	}
	if (size == 2 && str[0] != '-')
		str[0] = c;
}

char	*ft_itoa(int n)
{
	char	*number;
	int		size;

	size = size_itoa(n);
	number = malloc(sizeof(char) * size);
	if (!number)
		return (NULL);
	number[0] = '\0';
	write_itoa(n, number, size);
	number[size - 1] = '\0';
	return (number);
}
