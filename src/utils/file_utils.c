/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 20:58:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/12 16:58:14 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static void	print_file_error(char *filepath, int err, t_ctx *ctx)
{
	(void)err;
	ctx->error_code = 1;
	print_error(filepath);
}

int	file_open_read(char *filepath, t_ctx *ctx)
{
	int	fd;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		print_file_error(filepath, errno, ctx);
	return (fd);
}

int	file_open_write(char *filepath, t_ctx *ctx)
{
	int	fd;

	fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		print_file_error(filepath, errno, ctx);
	return (fd);
}

int	file_open_append(char *filepath, t_ctx *ctx)
{
	int	fd;

	fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		print_file_error(filepath, errno, ctx);
	return (fd);
}

int	check_spaces_in_filename(char *filename)
{
	int	open_quotes[2];

	open_quotes[0] = 0;
	open_quotes[1] = 0;
	while (*filename)
	{
		update_open_quotes(*filename, open_quotes);
		if (*filename == ' ' && !open_quotes[1])
			return (1);
		filename++;
	}
	return (0);
}
