/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:24:18 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 22:01:45 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
int	builtin_cd(t_cmd *cmd, t_list **envs)
{
	(void)envs;
	printf("<CD>\n");
	char *path;
	if (!cmd->args[1])
	{
		path = getenv("HOME");
		if (!path)
			printf("home not set");
	}
	else
		path = cmd->args[1];

	return (chdir(path));
}
