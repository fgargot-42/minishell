/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 19:07:32 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/21 23:45:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

static void	exec_pipe_command(t_node *node, t_list **envs, t_ctx *ctx)
{
	char	*path;
	char	**char_envs;

	if (node->type == NODE_GROUP)
	{
		exec(node, envs, ctx);
		exit(ctx->error_code);
	}
	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
		exit(1);
	expand_cmd_args(node, envs, ctx);
	if (is_builtin(node->cmd))
		exit(call_builtin(node, envs, ctx));
	path = get_command_path(node->cmd->args[0], *envs);
	char_envs = reconstruct_envs(*envs);
	if (path && char_envs)
		execve(path, node->cmd->args, char_envs);
	exit_fork_clean(node, char_envs, path);
}

static pid_t	exec_left_pipe_cmd(t_node *node, t_list **envs, int fd_read,
					t_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close(fd_read);
		apply_redirect(node->fd_in, STDIN_FILENO);
		apply_redirect(node->fd_out, STDOUT_FILENO);
		exec_pipe_command(node, envs, ctx);
	}
	return (pid);
}

static pid_t	exec_right_pipe_cmd(t_node *node, t_list **envs, t_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		apply_redirect(node->fd_in, STDIN_FILENO);
		apply_redirect(node->fd_out, STDOUT_FILENO);
		exec_pipe_command(node, envs, ctx);
	}
	return (pid);
}

static int	update_pipe_exit_status(int *pid, int status)
{
	int	new_status;

	new_status = 1;
	if (pid[0] > 0)
		waitpid(pid[0], NULL, 0);
	if (pid[1] <= 0)
		return (status);
	waitpid(pid[1], &status, 0);
	if (WIFEXITED(status))
		new_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		new_status = 128 + WTERMSIG(status);
	return (new_status);
}

int	exec_pipeline(t_node *node, t_list **envs, t_ctx *ctx)
{
	int		status;
	int		fd[2];
	pid_t	pid[2];
	int		redir_invalid;

	if (pipe(fd) == -1)
	{
		ft_putstr_fd("minishell: pipe failed\n", 2);
		return (1);
	}
	pid[0] = -1;
	pid[1] = 0;
	redir_invalid = resolve_pipe_redirs(node, envs, ctx, fd);
	status = redir_invalid == 2;
	if (!(redir_invalid & 1))
		pid[0] = exec_left_pipe_cmd(node->left, envs, node->right->fd_in, ctx);
	cleanup_node_fds(node->left);
	if (node->right->type == NODE_PIPE)
		status = exec_pipeline(node->right, envs, ctx);
	else if (!(redir_invalid & 2))
		pid[1] = exec_right_pipe_cmd(node->right, envs, ctx);
	cleanup_node_fds(node->right);
	status = update_pipe_exit_status(pid, status);
	return (status);
}
