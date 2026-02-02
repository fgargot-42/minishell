/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 18:40:23 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 22:06:58 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "string.h"
#include <unistd.h>

int	is_builtin(t_cmd *cmd)
{
	int			i;
	const char	*c = cmd->args[0];
	const char	*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit"};

	i = 0;
	while (i < 7)
	{
		if (strcmp(c, cmds[i++]) == 0)
			return (1);
	}
	return (0);
}

int	call_builtin(t_node *node, t_list **envs, t_ctx *ctx)
{
	char			*c;
	int				i;
	int				err_code;
	int				stdio_fd[2];
	const char		*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env",
				"exit"};
	t_builtin_func	cmds_func[] = {builtin_echo, builtin_cd, builtin_pwd,
			builtin_export, builtin_unset, builtin_env, builtin_exit};

	c = node->cmd->args[0];
	i = 0;
	err_code = 0;
	stdio_fd[0] = dup(0);
	stdio_fd[1] = dup(1);
	dup2(node->fd_out, STDOUT_FILENO);
	dup2(node->fd_in, STDIN_FILENO);
	while (i < 7)
	{
		if (strcmp(c, cmds[i]) == 0)
		{
			err_code = cmds_func[i](node->cmd, envs, ctx);
			break ;
		}
		i++;
	}
	dup2(stdio_fd[0], STDIN_FILENO);
	dup2(stdio_fd[1], STDOUT_FILENO);
	close(stdio_fd[0]);
	close(stdio_fd[1]);
	return (err_code);
}
