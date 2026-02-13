/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 20:58:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/13 21:17:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "libft.h"

static void	print_file_error(char *filepath, int err)
{
	char	*join;

	join = ft_strjoin("minishell: ", filepath);
	errno = err;
	if (join)
	{
		perror(join);
		free(join);
	}
	else
		perror("minishell");
}

int	file_open_read(char *filepath)
{
	int		fd;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		print_file_error(filepath, errno);
	return (fd);
}

int	file_open_write(char *filepath)
{
	int		fd;
	
	fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		print_file_error(filepath, errno);
	return (fd);
}

int	file_open_append(char *filepath)
{
	int		fd;

	fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		print_file_error(filepath, errno);
	return (fd);
}
