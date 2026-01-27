#include "minishell.h"
#include <sys/types.h>
#include <unistd.h>
int	exec(t_node *root)
{
	int code;
	if (root->type == NODE_CMD)
		return (exec_command(root->cmd));
	else if (root->type == NODE_PIPE)
	{
		exec(root->left);
		//pipe
		return (exec(root->right));
	}
	else if (root->type == NODE_OR)
	{
		code = exec(root->left);
		if (code != 0)
			return (exec(root->right));
	}
	else if (root->type == NODE_AND)
	{
		code = exec(root->left);
		if (code == 0)
			return (exec(root->right));
	}
	return (42);
}
