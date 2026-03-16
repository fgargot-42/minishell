/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 21:50:29 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/16 22:07:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static void	check_is_dir(char *filepath)
{
	struct stat	stat_buf;
	int			status;

	status = stat(filepath, &stat_buf);
	if (status != 0)
		return ;
	if (stat_buf.st_mode & S_IFDIR)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filepath, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		errno = EISDIR;
	}
}

static void	check_file_access(char *filepath)
{
	if (access(filepath, F_OK) || access(filepath, X_OK))
	{
		if (errno == EACCES)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filepath, 2);
			ft_putstr_fd(": Permission denied\n", 2);
		}
		if (errno == ENOENT)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filepath, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}
	else
		check_is_dir(filepath);
}

void	exit_fork_clean(t_node *node, char **char_envs, char *path,
			t_ctx *ctx)
{
	char	*is_path;

	free(path);
	free_string_array(char_envs);
	if (!node->cmd->args[0])
		return ;
	is_path = ft_strrchr(node->cmd->args[0], '/');
	if (!ft_strncmp(node->cmd->args[0], ".", 2))
	{
		ft_putstr_fd("minishell: filename argument required", 2);
		exit(2);
	}
	errno = 0;
	if (is_path)
		check_file_access(node->cmd->args[0]);
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	free_tree(ctx->cmd_tree);
	if (errno == EACCES || errno == EISDIR)
		exit(126);
	exit(127);
}

void	builtin_fork_clean(t_list **envs, t_ctx *ctx)
{
	ft_lstclear(envs, env_free);
	free_tree(ctx->cmd_tree);
}
