/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 19:07:32 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 22:19:53 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	exec_pipe_command(t_node *node, t_list **envs, t_ctx *ctx)
{
	char	*path;
	const char	**char_envs = reconstruct_envs(*envs);


	if (DEBUG)
		print_str_list(node->cmd->args);
	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
		return (-1);
	// check les buitlitns ici
	
	for (int i = 0; node->cmd->args[i]; i++)
	{

		char *expanded = expand_var(node->cmd->args[i], *envs, ctx);
		if (expanded != node->cmd->args[i])
		{
			free(node->cmd->args[i]);
			node->cmd->args[i] = expanded;
		}
	}

	if (is_builtin(node->cmd))
		exit(call_builtin(node, envs, ctx));

	path = find_in_path(node->cmd->args[0]);
	execve(path, node->cmd->args, (char *const *)char_envs);
	exit(127);
}

pid_t	exec_left_pipe_cmd(t_node *node, t_list **envs, int read_fd, t_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		printf("%p\n", node);
		close(read_fd);
		dup2(node->fd_out, STDOUT_FILENO);
		close(node->fd_out);
		if (node->fd_in != STDIN_FILENO)
		{
			dup2(node->fd_in, 0);
			close(node->fd_in);
		}
		exec_pipe_command(node, envs, ctx);
	}
	return (pid);
}

pid_t	exec_right_pipe_cmd(t_node *node, t_list **envs, t_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		printf("%p\n", node);
		dup2(node->fd_in, STDIN_FILENO);
		close(node->fd_in);
		if (node->fd_out != STDOUT_FILENO)
		{
			dup2(node->fd_out, 1);
			close(node->fd_out);
		}
		exec_pipe_command(node, envs, ctx);
	}
	return (pid);
}

int	exec_pipeline(t_node *node, t_list **envs, t_ctx *ctx)
{
	int	status;
	int	fd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	pipe(fd);
	resolve_redirs(node->left);

	if (node->left->fd_out == STDOUT_FILENO)
		node->left->fd_out = fd[1];
	if (node->left->fd_in == STDIN_FILENO)
		node->left->fd_in = node->fd_in;

	resolve_redirs(node->right);

	if (node->right->fd_in == STDIN_FILENO)
		node->right->fd_in = fd[0];
	if (node->right->fd_out == STDOUT_FILENO)
		node->right->fd_out = node->fd_out;
	pid_left = exec_left_pipe_cmd(node->left, envs, fd[0], ctx);
	pid_right = 0;
	close(fd[1]);
	if (node->right->type == NODE_PIPE)
		status = exec_pipeline(node->right, envs, ctx);
	else
		pid_right = exec_right_pipe_cmd(node->right, envs, ctx);
	close(fd[0]);
	waitpid(pid_left, NULL, 0);
	if (pid_right)
	{
		waitpid(pid_right, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (status);
}
