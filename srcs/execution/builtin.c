/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 18:40:23 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 18:42:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "string.h"

int	is_builtin(t_cmd *cmd)
{
	int			i;
	const char	*c = cmd->args[0];
	const char	*cmds[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit"
	};

	i = 0;
	while (i < 7)
	{
		if (strcmp(c, cmds[i++]) == 0)
			return (1);
	}
	return (0);
}

int	call_builtin(t_cmd *cmd, t_list **envs)
{
	char	*c;
	int		i;
	const char *cmds[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit"
	};

	c = cmd->args[0];
	t_builtin_func cmds_func[] = {
		builtin_echo,
		builtin_cd,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
		builtin_exit
	};
	i = 0;
	while (i < 7)
	{
		if (strcmp(c, cmds[i]) == 0)
			return (cmds_func[i](cmd, envs));
		i++;
	}
	return (43);
}
