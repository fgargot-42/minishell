/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:43:03 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/21 19:39:05 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

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
