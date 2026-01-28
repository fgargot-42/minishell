#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
int	exec_pipeline(t_node *node, t_env *envs)
{
	int fd[2];
	int status;
	pid_t	pid_left;
	pid_t	pid_right;

	pipe(fd);

	pid_left = fork();
	if (pid_left == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		return (exec(node->left, envs));
	}

	pid_right = fork();
	if (pid_right == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		return (exec(node->right, envs));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (42);
}
