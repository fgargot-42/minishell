/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:33:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/07 18:29:39 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

static int	check_is_long(char *str, int sign)
{
	size_t		i;
	int			is_long;
	const char	*long_max = "9223372036854775807";

	is_long = 1;
	i = 0;
	while (*str)
	{
		if (ft_isdigit(*str))
		{
			if (sign && (i == ft_strlen(long_max) - 1))
				is_long &= (*str <= (long_max[i] + 1));
			else
				is_long &= (*str <= long_max[i]);
			i++;
		}
		else if (*str != '\"')
			return (0);
		str++;
	}
	if (i < ft_strlen(long_max))
		return (1);
	return (is_long);
}

static int	ft_is_long(char *str)
{
	int	sign;
	int	is_long;

	sign = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	sign = *str == '-';
	if (*str == '+' || *str == '-')
		str++;
	is_long = check_is_long(str, sign);
	return (is_long);
}

static	long	ft_atol(char *str)
{
	long	nb;
	int		sign;

	nb = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while ((*str >= '0' && *str <= '9') || *str == '\"')
	{
		if (*str != '\"')
			nb = nb * 10 + *str - '0';
		str++;
	}
	return (nb * sign);
}

int	builtin_exit(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	int		err_code;

	(void)envs;
	err_code = ctx->error_code;
	ft_putstr_fd("exit\n", 2);
	if (cmd->args[1] && cmd->args[2])
		exit(1);
	if (cmd->args[1])
	{
		if (!ft_is_long(cmd->args[1]))
			exit(2);
		err_code = ft_atol(cmd->args[1]);
	}
	exit(err_code & 0xff);
}
