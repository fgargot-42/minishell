/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 21:43:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 21:51:35 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_env_node_by_key(t_list *env_list, char *key)
{
	t_list	*node;

	node = env_list;
	while (node)
	{
		if (!ft_strcmp(((t_env *)node->content)->key, key))
			return (node);
		node = node->next;
	}
	return (NULL);
}
