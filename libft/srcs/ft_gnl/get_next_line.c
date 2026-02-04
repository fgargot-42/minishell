/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 20:03:59 by fgargot           #+#    #+#             */
/*   Updated: 2025/12/11 20:53:48 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <unistd.h>

static char	*ft_strcat(char *dest, const char *src, size_t n)
{
	char	*res;
	size_t	dest_len;
	size_t	i;

	if (!src)
		return (dest);
	dest_len = 0;
	if (dest)
		dest_len = ft_strlen(dest);
	i = 0;
	res = ft_calloc(sizeof(char), dest_len + n + 1);
	while (res && i < dest_len + n)
	{
		if (i < dest_len)
			res[i] = dest[i];
		else
			res[i] = src[i - dest_len];
		i++;
	}
	if (res)
		res[i] = '\0';
	if (dest)
		free(dest);
	return (res);
}

static void	*ft_memmove_to_start(void *dest, size_t index, size_t size)
{
	unsigned char	*d;
	size_t			i;

	d = (unsigned char *)dest;
	i = 0;
	while (i < size)
	{
		if (i < size - index)
			d[i] = d[i + index];
		else
			d[i] = 0;
		i++;
	}
	return (dest);
}

static int	initialize_buffer(char **buffer)
{
	int	i;

	i = 0;
	if (!*buffer)
	{
		*buffer = ft_calloc(sizeof(char), BUFFER_SIZE);
		if (!*buffer)
			return (-1);
		return (0);
	}
	while (i < BUFFER_SIZE)
	{
		if ((*buffer)[i] == '\n' && i != BUFFER_SIZE - 1)
		{
			i++;
			ft_memmove_to_start(*buffer, i, BUFFER_SIZE);
			break ;
		}
		i++;
	}
	return (BUFFER_SIZE - i);
}

static char	*read_next_line(int fd, char **buffer, char **res, int i)
{
	int	end_file;
	int	j;

	end_file = 0;
	while (!end_file)
	{
		j = 0;
		while (j < i && (*buffer)[j] && (*buffer)[j] != '\n')
			j++;
		if (j < i && (*buffer)[j] == '\n')
			j++;
		*res = ft_strcat(*res, *buffer, j);
		if (j && (*buffer)[j - 1] == '\n')
			return (*res);
		i = read(fd, *buffer, BUFFER_SIZE);
		if (i > 0 && i < BUFFER_SIZE)
			(*buffer)[i] = '\0';
		if (i <= 0)
			end_file = 1;
	}
	free(*buffer);
	*buffer = 0;
	return (*res);
}

char	*get_next_line(int fd)
{
	char			*res;
	static char		*buffer[1024];
	int				i;

	res = NULL;
	if (fd < 0 || fd > 1023)
		return (NULL);
	i = initialize_buffer(&buffer[fd]);
	if (i == -1)
	{
		if (res)
			free(res);
		return (NULL);
	}
	read_next_line(fd, &buffer[fd], &res, i);
	if (!buffer[fd] && (!res || !res[0]))
	{
		if (res)
			free(res);
		return (NULL);
	}
	return (res);
}
