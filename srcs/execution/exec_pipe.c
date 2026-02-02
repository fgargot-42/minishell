#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	exec_pipeline(t_node *node, t_list **envs, t_ctx *ctx)
{
	int	fd_stdin;
	int	fd_stdout;
	int	status;

	fd_stdin = dup(STDIN_FILENO);
	fd_stdout = dup(STDOUT_FILENO);
	pipe(ctx->fd);
	dup2(ctx->fd[1], STDOUT_FILENO);
	exec(node->left, envs, ctx);
	dup2(ctx->fd[0], STDIN_FILENO);
	status = exec(node->right, envs, ctx);
	dup2(fd_stdin, STDIN_FILENO);
	dup2(fd_stdout, STDOUT_FILENO);
	close(fd_stdin);
	close(fd_stdout);
	return (status);
}
