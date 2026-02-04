/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:18:37 by fgargot           #+#    #+#             */
/*   Updated: 2025/12/11 21:07:12 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_printf.h"
#include "libft.h"

int	ft_print_d(va_list args)
{
	int		i;
	char	*res;

	i = va_arg(args, int);
	res = ft_itoa(i);
	i = write(1, res, ft_strlen(res));
	free(res);
	return (i);
}

int	ft_print_i(va_list args)
{
	int		i;
	char	*res;

	i = va_arg(args, int);
	res = ft_itoa(i);
	i = write(1, res, ft_strlen(res));
	free(res);
	return (i);
}

int	ft_print_u(va_list args)
{
	unsigned int	nb;
	unsigned int	tmp;
	char			c[11];
	int				i;

	i = 0;
	nb = va_arg(args, unsigned int);
	tmp = nb;
	while (tmp > 0)
	{
		tmp /= 10;
		i++;
	}
	if (i == 0)
		i = 1;
	c[i] = '\0';
	while (i > 0)
	{
		c[i - 1] = nb % 10 + '0';
		nb /= 10;
		i--;
	}
	i = write(1, c, ft_strlen(c));
	return (i);
}

int	ft_print_xlower(va_list args)
{
	int		nb;
	char	*res;
	int		i;

	nb = va_arg(args, int);
	res = ft_itoa_hex(nb);
	if (!res)
		return (0);
	i = 0;
	while (res[i] == '0' && res[i + 1])
		i++;
	nb = write(1, &res[i], ft_strlen(&res[i]));
	free(res);
	return (nb);
}

int	ft_print_xupper(va_list args)
{
	long	nb;
	char	*res;
	int		i;

	nb = va_arg(args, long);
	res = ft_itoa_hex(nb);
	if (!res)
		return (0);
	nb = 0;
	while (res[nb])
	{
		res[nb] = ft_toupper(res[nb]);
		nb++;
	}
	i = 0;
	while (res[i] == '0' && res[i + 1])
		i++;
	nb = write(1, &res[i], ft_strlen(&res[i]));
	free(res);
	return (nb);
}
