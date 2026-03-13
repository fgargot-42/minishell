/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:23:39 by mabarrer          #+#    #+#             */
/*   Updated: 2026/03/12 20:47:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	check_envname_format(char *str)
{
	if (!str || !*str)
		return (1);
	if (!ft_isalpha(*str) && *str != '_')
		return (1);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
		{
			if (*str == '=' || (*str == '+' && *(str + 1) == '='))
				return (0);
			return (1);
		}
		str++;
	}
	return (0);
}

int	add_quoted_arg_value(char **arg, char **env)
{
	int		i;
	char	*tmp;

	i = 1;
	while (env[i])
	{
		tmp = ft_strjoin(*arg, " ");
		free(*arg);
		*arg = ft_strjoin(tmp, env[i]);
		free(tmp);
		if (!*arg || ft_strchr(env[i], '\"'))
			break ;
		i++;
	}
	return (i);
}

static int	builtin_export_add(t_cmd *cmd, t_list **envs)
{
	int		i;
	int		status;
	int		is_error;

	i = 1;
	is_error = 0;
	while (cmd->args[i])
	{
		status = check_envname_format(cmd->args[i]);
		if (status)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			is_error = 1;
			i++;
			continue ;
		}
		add_env(&cmd->args[i], envs);
		i++;
	}
	return (is_error);
}

int	builtin_export(t_cmd *cmd, t_list **envs, t_ctx *ctx)
{
	int		status;

	(void)ctx;
	if (!envs)
		return (0);
	if (!*envs && !cmd->args[1])
		return (0);
	if (!cmd->args[1])
	{
		builtin_export_print(envs);
		return (0);
	}
	status = builtin_export_add(cmd, envs);
	return (status);
}
