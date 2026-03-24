/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 21:06:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/24 21:19:36 by fgargot          ###   ########.fr       */
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

void	inherit_redirs(t_node *node, t_node *parent)
{
	if (!parent)
		return ;
	if (node == parent->left)
	{
		if (node->fd_in != parent->fd_in && parent->fd_in != STDIN_FILENO)
		{
			if (node->fd_in != STDIN_FILENO)
				close(node->fd_in);
			node->fd_in = parent->fd_in;
		}
	}
	if (node == parent->right || parent->type == NODE_GROUP
		|| parent->type == NODE_AND || parent->type == NODE_OR)
	{
		if (node->fd_out != parent->fd_out && parent->fd_out != STDOUT_FILENO)
		{
			if (node->fd_out != STDOUT_FILENO)
				close(node->fd_out);
			node->fd_out = parent->fd_out;
		}
	}
}

void	exec(t_node *root, t_node *parent, t_list **envs, t_ctx *ctx)
{
	if (resolve_redirs(root, *envs, ctx))
		return ;
	inherit_redirs(root, parent);
	if (root->type == NODE_CMD)
		ctx->error_code = exec_command(root, parent, envs, ctx);
	else if (root->type == NODE_PIPE)
		ctx->error_code = exec_pipeline(root, envs, ctx);
	else if (root->type == NODE_OR)
	{
		exec(root->left, root, envs, ctx);
		if (ctx->error_code != 0 && ctx->error_code != 130)
			exec(root->right, root, envs, ctx);
	}
	else if (root->type == NODE_AND)
	{
		exec(root->left, root, envs, ctx);
		if (ctx->error_code == 0)
			exec(root->right, root, envs, ctx);
	}
	else if (root->type == NODE_GROUP)
		exec(root->left, root, envs, ctx);
	if (root->type != NODE_CMD)
		cleanup_node_fds(root, parent);
}
