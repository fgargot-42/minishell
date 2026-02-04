/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:41:01 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/22 21:29:55 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	char	digit;

	if (fd < 0)
		return ;
	digit = n % 10 + '0';
	if (n < 0)
	{
		write(fd, "-", 1);
		digit = -(n % 10) + '0';
		if (n < -9)
			ft_putnbr_fd(-(n / 10), fd);
	}
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	write(fd, &digit, 1);
}
