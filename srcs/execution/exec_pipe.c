/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 19:07:32 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/31 19:57:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>

pid_t	exec_left_pipe_cmd(t_node *node, t_list **envs, int read_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		return (-1);
	}
	if (pid == 0)
	{
		close(read_fd);
		dup2(node->fd_out, STDOUT_FILENO);
		close(node->fd_out);
		//close(node->fd_in);
		exit(exec(node, envs));
	}
	return (pid);
}

pid_t	exec_right_pipe_cmd(t_node *node, t_list **envs, int write_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		return (-1);
	}
	if (pid == 0)
	{
		close(write_fd);
		dup2(node->fd_in, STDIN_FILENO);
		//close(node->fd_out);
		close(node->fd_in);
		exit(exec(node, envs));
	}
	return (pid);
}

int	exec_pipeline(t_node *node, t_list **envs)
{
	int fd[2];
	int status;
	pid_t	pid_left;
	pid_t	pid_right;

	pipe(fd);
	node->left->fd_in = node->fd_in;
	node->left->fd_out = fd[1];
	node->right->fd_in = fd[0];
	node->right->fd_out = node->fd_out;
	pid_left = exec_left_pipe_cmd(node->left, envs, fd[0]);
	pid_right = 0;
	close(fd[0]);
	if (node->right->type == NODE_PIPE)
		status = exec_pipeline(node->right, envs);
	else
		pid_right = exec_right_pipe_cmd(node->right, envs, fd[1]);
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
