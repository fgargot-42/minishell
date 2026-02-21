/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:41:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/20 22:28:04 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void	cleanup_node_fds(t_node *node)
{
	if (!node)
		return ;
	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		node->fd_in = STDIN_FILENO;
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		node->fd_out = STDOUT_FILENO;
	}
}

static t_redir	*new_redir(t_token **tokens)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = (*tokens)->type;
	redir->next = NULL;
	*tokens = (*tokens)->next;
	redir->file = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
	return (redir);
}

void	add_redirection(t_redir **redir, t_token **tokens)
{
	t_redir	*tmp;
	t_redir	*new_redirection;

	new_redirection = new_redir(tokens);
	if (!new_redirection)
		return ;
	if (!*redir)
	{
		*redir = new_redirection;
		return ;
	}
	tmp = *redir;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redirection;
}
