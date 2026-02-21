/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 21:06:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/21 23:45:20 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <unistd.h>

void	apply_redirect(int node_fd, int std_fd)
{
	if (node_fd != std_fd)
	{
		dup2(node_fd, std_fd);
		close(node_fd);
	}
}

void	propagate_redirs(t_node *node)
{
	if (node->left)
	{
		node->left->fd_in = node->fd_in;
		if (node->type == NODE_GROUP || node->type == NODE_OR
			|| node->type == NODE_AND)
			node->left->fd_out = node->fd_out;
	}
	if (node->right)
	{
		if (node->type == NODE_GROUP || node->type == NODE_OR
			|| node->type == NODE_AND)
			node->right->fd_in = node->fd_in;
		node->right->fd_out = node->fd_out;
	}
}

void	exec(t_node *root, t_list **envs, t_ctx *ctx)
{
	if (resolve_redirs(root, *envs, ctx))
	{
		ctx->error_code = 1;
		return ;
	}
	propagate_redirs(root);
	if (root->type == NODE_CMD)
		ctx->error_code = exec_command(root, envs, ctx);
	else if (root->type == NODE_PIPE)
		ctx->error_code = exec_pipeline(root, envs, ctx);
	else if (root->type == NODE_OR)
	{
		exec(root->left, envs, ctx);
		if (ctx->error_code != 0)
			exec(root->right, envs, ctx);
	}
	else if (root->type == NODE_AND)
	{
		exec(root->left, envs, ctx);
		if (ctx->error_code == 0)
			exec(root->right, envs, ctx);
	}
	else if (root->type == NODE_GROUP)
		exec(root->left, envs, ctx);
}
