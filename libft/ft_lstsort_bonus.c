/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 01:54:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/29 02:54:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_lstswap(t_list **node1, t_list **node2,
	t_list *prev1, t_list *prev2)
{
	t_list	*tmp;

	if (prev1)
		prev1->next = *node2;
	if (prev2)
		prev2->next = *node1;
	tmp = (*node1)->next;
	(*node1)->next = (*node2)->next;
	(*node2)->next = tmp;
	tmp = *node2;
	*node2 = *node1;
	*node1 = tmp;
}

void	ft_lstsort(t_list **lst, int (*cmp)(void *, void *))
{
	t_list	*node1;
	t_list	*node2;
	t_list	*node1_prev;
	t_list	*node2_prev;

	node1 = *lst;
	node1_prev = NULL;
	while (node1->next)
	{
		node2 = node1->next;
		node2_prev = node1;
		while (node2)
		{
			if (cmp(node1->content, node2->content) > 0)
			{
				ft_lstswap(&node1, &node2, node1_prev, node2_prev);
				if (node1_prev == NULL)
					*lst = node1;
			}
			node2_prev = node2;
			node2 = node2->next;
		}
		node1_prev = node1;
		node1 = node1->next;
	}
}
