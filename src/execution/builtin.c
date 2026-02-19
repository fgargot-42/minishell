/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 18:40:23 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 21:32:17 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "string.h"
#include <unistd.h>

int	get_builtin_i(const char *cmd)
{
	const char	*cmds[] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit"};
	int			i;

	i = 0;
	while (i < 7)
	{
		if (strcmp(cmd, cmds[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	is_builtin(t_cmd *cmd)
{
	if (!cmd->args[0])
		return (0);
	return (get_builtin_i(cmd->args[0]) >= 0);
}

int	call_builtin(t_node *node, t_list **envs, t_ctx *ctx)
{
	const t_builtin_func	cmds_func[] = {builtin_echo, builtin_cd,
		builtin_pwd, builtin_export, builtin_unset, builtin_env, builtin_exit};
	int						stdio_fd[2];
	int						err_code;
	int						i;

	i = get_builtin_i(node->cmd->args[0]);
	err_code = 0;
	stdio_fd[0] = dup(0);
	stdio_fd[1] = dup(1);
	dup2(node->fd_out, STDOUT_FILENO);
	dup2(node->fd_in, STDIN_FILENO);
	if (i >= 0)
		err_code = cmds_func[i](node->cmd, envs, ctx);
	dup2(stdio_fd[0], STDIN_FILENO);
	dup2(stdio_fd[1], STDOUT_FILENO);
	close(stdio_fd[0]);
	close(stdio_fd[1]);
	return (err_code);
}
