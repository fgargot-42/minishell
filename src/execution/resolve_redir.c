/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:04:18 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/12 16:56:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>

static int	open_redir(char **filenames, t_redir *redir, t_list *envs,
				t_ctx *ctx)
{
	int		new_fd;

	if (!filenames || !filenames[0])
	{
		ft_putstr_fd("minishell: : no such file or directory", 2);
		return (-1);
	}
	if (filenames[1])
	{
		ft_putstr_fd("minishell: ambiguous redirect", 2);
		return (-1);
	}
	new_fd = -1;
	if (redir->type == TOKEN_REDIR_IN)
		new_fd = file_open_read(filenames[0], ctx);
	else if (redir->type == TOKEN_REDIR_OUT)
		new_fd = file_open_write(filenames[0], ctx);
	else if (redir->type == TOKEN_APPEND)
		new_fd = file_open_append(filenames[0], ctx);
	else if (redir->type == TOKEN_HEREDOC)
		new_fd = handle_heredoc(filenames, envs, ctx);
	return (new_fd);
}

static void	update_node_fds(t_node *node, t_redir *redir, int fd)
{
	if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
	{
		if (node->fd_out != STDOUT_FILENO)
			close(node->fd_out);
		node->fd_out = fd;
	}
	else
	{
		if (node->fd_in != STDIN_FILENO)
			close(node->fd_in);
		node->fd_in = fd;
	}
}

int	resolve_redirs(t_node *node, t_list *envs, t_ctx *ctx)
{
	t_redir	*redir;
	char	**expanded;
	int		new_fd;
	char	*tmp;

	if (!node)
		return (0);
	redir = node->redirs;
	while (redir)
	{
		expanded = expand_wildcards(redir->file);
		expand_var(&expanded[0], envs, ctx);
		tmp = remove_quotes(expanded[0]);
		free(expanded[0]);
		expanded[0] = tmp;
		new_fd = open_redir(expanded, redir, envs, ctx);
		free_string_array(expanded);
		if (new_fd == -1)
			cleanup_node_fds(node, NULL);
		if (new_fd == -1)
			return (1);
		update_node_fds(node, redir, new_fd);
		redir = redir->next;
	}
	return (0);
}
