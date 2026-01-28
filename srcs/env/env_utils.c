#include "minishell.h"

t_env *envlist_last(t_env *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	envlist_addback(t_env **lst, t_env *new)
{
	t_env	*last;

	if (!lst)
		return ;
	last = envlist_last(*lst);
	if (!last)
		*lst = new;
	else
		last->next = new;
}



