#include "minishell.h"

t_node	*create_node(t_node_type type, t_node *left, t_node *right)
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
	return (node);
}

t_node	*create_cmd_node(t_cmd *cmd)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
	    return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->fd_in = 0;
	node->fd_out = 1;
	node->cmd = cmd;
	return (node);
}
void	print_tree(t_node *node, int d)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i++ < d)
		printf("  ");
	if (node->type == NODE_CMD)
	{
		printf("CMD: ");
		if (node->cmd && node->cmd->args && node->cmd->args[0])
			printf("%s\n", node->cmd->args[0]);
		else
			printf("(empty)\n");
	}
	else if (node->type == NODE_PIPE)
		printf("PIPE\n");
	else if (node->type == NODE_AND)
		printf("AND\n");
	else if (node->type == NODE_OR)
		printf("OR\n");
	else if (node->type == NODE_GROUP)
		printf("GROUP\n");
	print_tree(node->left, d + 1);
	print_tree(node->right, d + 1);
}
