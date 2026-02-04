/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:10:51 by fgargot           #+#    #+#             */
/*   Updated: 2025/10/22 21:26:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list	*map;
	t_list	*curr;
	void	*content;

	if (!f || !del)
		return (NULL);
	map = NULL;
	while (lst)
	{
		content = (*f)(lst->content);
		curr = ft_lstnew(content);
		if (!curr)
		{
			del(content);
			ft_lstclear(&map, del);
			return (NULL);
		}
		ft_lstadd_back(&map, curr);
		lst = lst->next;
	}
	return (map);
}
