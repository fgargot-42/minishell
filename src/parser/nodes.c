/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 21:30:31 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/20 22:12:22 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node(t_node_type type, t_node *left, t_node *right,
	t_redir *redir)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = left;
	node->right = right;
	node->fd_in = 0;
	node->fd_out = 1;
	node->cmd = NULL;
	node->redirs = redir;
	return (node);
}

t_node	*create_cmd_node(t_cmd *cmd, t_redir *redir)
{
	t_node	*node;

	if (!cmd)
		return (NULL);
	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->fd_in = 0;
	node->fd_out = 1;
	node->cmd = cmd;
	node->redirs = redir;
	return (node);
}
