/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:43:03 by mabarrer          #+#    #+#             */
/*   Updated: 2026/03/23 20:24:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "libft.h"

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	update_open_quotes(char c, int *open_quotes)
{
	if (c == '\'' && !open_quotes[1])
		open_quotes[0] = !open_quotes[0];
	if (c == '\"' && !open_quotes[0])
		open_quotes[1] = !open_quotes[1];
}

void	print_cmd_error(char *cmd_name, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

void	print_error(char *msg)
{
	char	*err_msg;
	int		error;

	error = errno;
	err_msg = ft_strjoin("minishell: ", msg);
	errno = error;
	if (err_msg)
		perror(err_msg);
	else
		perror("minishell");
	free(err_msg);
}
