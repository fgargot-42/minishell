#include "minishell.h"
#include <sys/types.h>
#include <unistd.h>
int	exec(t_node *root, t_env *envs)
{
	int code;

	code = 42;
	if (root->type == NODE_CMD)
	{
		print_redirs(root->cmd->redirs);
		code = exec_command(root->cmd, envs);
	}
	else if (root->type == NODE_PIPE)
	{
		return (exec_pipeline(root, envs));
	}
	else if (root->type == NODE_OR)
	{
		code = exec(root->left, envs);
		if (code != 0)
			code = exec(root->right, envs);
	}
	else if (root->type == NODE_AND)
	{
		code = exec(root->left, envs);
		if (code == 0)
			code = exec(root->right,envs);
	}
	return (code);
}
