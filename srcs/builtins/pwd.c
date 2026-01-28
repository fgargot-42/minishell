/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:24:41 by mabarrer          #+#    #+#             */
/*   Updated: 2026/01/28 22:07:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
int	builtin_pwd(t_cmd *cmd, t_list **envs)
{
	(void)cmd;
	(void)envs;
	char buf[1024];

	printf("<PWD>\n");
	if (getcwd(buf, sizeof(buf)))
		printf("%s\n", buf);

	return (0);
}
