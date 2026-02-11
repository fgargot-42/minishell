/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:25:22 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/11 20:34:12 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>


static void	print_connector(int depth, bool *last_child, bool is_last)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		if (i == depth - 1)
			printf(is_last ? CYAN"    └── "RESET : CYAN"    ├── "RESET);
		else
			printf(last_child[i] ? "        " : CYAN"    │   "RESET);
		i++;
	}
}

static void	print_node_label(t_node *node)
{
	if (node->type == NODE_CMD)
	{
		printf("⚡ "GREEN"CMD"RESET": ");
		if (node->cmd && node->cmd->args && node->cmd->args[0])
			printf(CYAN"%s"RESET"\n", node->cmd->args[0]);
		else
			printf(CYAN"(empty)"RESET"\n");
	}
	else if (node->type == NODE_PIPE)
		printf(CYAN"│ "BLUE"PIPE"RESET"\n");
	else if (node->type == NODE_AND)
		printf(YELLOW"&& AND"RESET"\n");
	else if (node->type == NODE_OR)
		printf(MAGENTA"|| OR"RESET"\n");
	else if (node->type == NODE_GROUP)
		printf(RED"( ) GROUP"RESET"\n");
}

static void	print_tree_recursive(t_node *node, int depth, bool *last_child)
{
	if (!node)
		return ;
	if (depth > 0)
		print_connector(depth, last_child, last_child[depth - 1]);
	print_node_label(node);
	if (node->left)
	{
		last_child[depth] = (node->right == NULL);
		print_tree_recursive(node->left, depth + 1, last_child);
	}
	if (node->right)
	{
		last_child[depth] = true;
		print_tree_recursive(node->right, depth + 1, last_child);
	}
}

void	print_tree_clean(t_node *node)
{
	bool	last_child[100];
	int		i;

	i = 0;
	while (i < 100)
		last_child[i++] = false;
	print_tree_recursive(node, 0, last_child);
}
