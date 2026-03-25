/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/25 23:11:05 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

void	exec_group_fork(t_node *node, t_node *parent, t_list **envs, t_ctx *ctx)
{
	exec(node, parent, envs, ctx);
	ft_lstclear(envs, env_free);
	free_tree(ctx->cmd_tree);
	exit(ctx->error_code);
}

static int	exec_command_fork(t_node *node, t_node *parent, t_list **envs,
				t_ctx *ctx)
{
	pid_t	pid;
	char	**char_envs;
	char	*path;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		path = get_command_path(node->cmd->args[0], *envs);
		char_envs = (char **)reconstruct_envs(*envs);
		ft_lstclear(envs, env_free);
		apply_redirect(node, parent);
		execve(path, node->cmd->args, char_envs);
		exit_fork_clean(node, char_envs, path, ctx);
	}
	if (pid > 0)
		waitpid(pid, &status, 0);
	return (status);
}

static int	check_cmd_node(t_node *node, t_node *parent)
{
	if (!node->cmd || !node->cmd->args || !node->cmd->args[0])
	{
		cleanup_node_fds(node, parent);
		return (1);
	}
	return (0);
}

int	exec_command(t_node *node, t_node *parent, t_list **envs, t_ctx *ctx)
{
	int		status;

	if (check_cmd_node(node, parent))
		return (0);
	expand_cmd_args(node, envs, ctx);
	if (!node->cmd->args || !node->cmd->args[0])
	{
		cleanup_node_fds(node, parent);
		return (0);
	}
	if (is_builtin(node->cmd))
	{
		status = call_builtin(node, envs, ctx);
		cleanup_node_fds(node, parent);
		return (status);
	}
	status = exec_command_fork(node, parent, envs, ctx);
	cleanup_node_fds(node, parent);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit\n", 2);
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}
