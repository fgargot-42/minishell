/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:32:41 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/20 19:04:32 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	long	nb;
	int		sign;

	nb = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9' && (nb * 10 + *nptr - '0') >= nb)
	{
		nb = nb * 10 + *nptr - '0';
		nptr++;
	}
	if ((*nptr >= '0' && *nptr <= '9'))
		return (-(sign + 1) / 2);
	return ((int)(nb * sign));
}
