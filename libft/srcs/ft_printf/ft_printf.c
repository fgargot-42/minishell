/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 14:21:52 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/28 19:48:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "ft_printf.h"
#include <unistd.h>

static int	ft_print_arg(char convert, va_list args)
{
	int	len;

	if (convert == 'c')
		len = ft_print_c(args);
	else if (convert == 's')
		len = ft_print_s(args);
	else if (convert == 'p')
		len = ft_print_p(args);
	else if (convert == 'd')
		len = ft_print_d(args);
	else if (convert == 'i')
		len = ft_print_i(args);
	else if (convert == 'u')
		len = ft_print_u(args);
	else if (convert == 'x')
		len = ft_print_xlower(args);
	else if (convert == 'X')
		len = ft_print_xupper(args);
	else
	{
		write(1, "%", 1);
		return (1);
	}
	return (len);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		i;
	int		len;

	va_start(args, s);
	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1])
		{
			len += ft_print_arg(s[i + 1], args);
			i++;
		}
		else
		{
			write(1, &s[i], 1);
			len++;
		}
		i++;
	}
	va_end(args);
	return (len);
}
