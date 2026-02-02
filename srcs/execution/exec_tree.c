/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 21:06:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 19:52:45 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <unistd.h>

int	exec(t_node *root, t_list **envs, t_ctx *ctx)
{
	int	code;

	code = 42;
	if (root->type == NODE_CMD)
	{
		if (DEBUG)
			print_redirs(root->cmd->redirs);
		code = exec_command(root, envs, ctx);
	}
	else if (root->type == NODE_PIPE)
		code = exec_pipeline(root, envs, ctx);
	else if (root->type == NODE_OR)
	{
		code = exec(root->left, envs, ctx);
		if (code != 0)
			code = exec(root->right, envs, ctx);
	}
	else if (root->type == NODE_AND)
	{
		code = exec(root->left, envs, ctx);
		if (code == 0)
			code = exec(root->right, envs, ctx);
	}
	else if (root->type == NODE_GROUP)
		code = exec(root->left, envs, ctx);
	return (code);
}
