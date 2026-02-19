/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:41:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 22:48:12 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void	cleanup_node_fds(t_node *node)
{
	if (!node)
		return ;
	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		node->fd_in = STDIN_FILENO;
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		node->fd_out = STDOUT_FILENO;
	}
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static t_redir	*new_redir(t_token **tokens)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = (*tokens)->type;
	redir->next = NULL;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		fprintf(stderr, "error redir not filename\n");
		free(redir);
		return (NULL);
	}
	redir->file = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
	return (redir);
}

void	add_redirection(t_cmd *cmd, t_token **tokens)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = new_redir(tokens);
	if (!redir)
		return ;
	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
}
