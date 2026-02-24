/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_redir_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:00:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/23 23:31:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	resolve_pipe_redirs(t_node *node, t_list **envs, t_ctx *ctx, int *fd)
{
	int	redir_invalid_left;
	int	redir_invalid_right;

	inherit_redirs(node->left, node);
	inherit_redirs(node->right, node);
	redir_invalid_left = resolve_redirs(node->left, *envs, ctx);
	if (node->left->fd_out == STDOUT_FILENO)
		node->left->fd_out = fd[1];
	else
		close(fd[1]);
	if (node->left->fd_in == STDIN_FILENO)
		node->left->fd_in = node->fd_in;
	redir_invalid_right = resolve_redirs(node->right, *envs, ctx);
	if (node->right->fd_in == STDIN_FILENO)
		node->right->fd_in = fd[0];
	else
		close(fd[0]);
	if (node->right->fd_out == STDOUT_FILENO)
		node->right->fd_out = node->fd_out;
	return (redir_invalid_left + redir_invalid_right * 2);
}
