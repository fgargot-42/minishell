/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 21:06:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/07 21:46:20 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <unistd.h>

void	exec(t_node *root, t_list **envs, t_ctx *ctx)
{
	if (root->type == NODE_CMD)
	{
		if (DEBUG)
			print_redirs(root->cmd->redirs);
		ctx->error_code = exec_command(root, envs, ctx);
	}
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
