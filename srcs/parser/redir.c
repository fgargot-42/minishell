/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:41:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 17:48:22 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static char	*get_type_name(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return ("<");
	if (type == TOKEN_REDIR_OUT)
		return (">");
	if (type == TOKEN_APPEND)
		return (">>");
	if (type == TOKEN_HEREDOC)
		return ("<<");
	return (NULL);
}

void	print_redirs(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	printf(CYAN"\n══════════════════════════ REDIRS ═══════════════════════════\n"RESET);
	printf(BLUE"⟩ "CYAN"["RESET);
	while (current)
	{
		printf("("YELLOW"%s"RESET": "MAGENTA"%s"RESET")",
			get_type_name(current->type), current->file);
		current = current->next;
		if (current)
			printf(BLUE", "RESET);
	}
	printf(CYAN"]\n"RESET);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

void	add_redirection(t_cmd *cmd, t_token **tokens)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = (t_redir *)malloc(sizeof(t_redir));
	redir->type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		fprintf(stderr, "error redir not filename\n");
		free(redir);
		return ;
	}
	redir->file = strdup((*tokens)->value);
	*tokens = (*tokens)->next;
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

int	resolve_redirs(t_node *node)
{
	t_redir	*redir;
	int		new_fd;

	if (!node->cmd)
		return (1);
	redir = node->cmd->redirs;
	while (redir)
	{
		new_fd = 0;
		if (redir->type == TOKEN_REDIR_IN)
			new_fd = open(redir->file, O_RDONLY);
		if (redir->type == TOKEN_REDIR_OUT)
			new_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->type == TOKEN_APPEND)
			new_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (new_fd == -1)
			return (1);
		if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
		{
			if (node->fd_out != STDOUT_FILENO)
				close(node->fd_out);
			node->fd_out = new_fd;
		}
		else
		{
			if (node->fd_in != STDIN_FILENO)
				close(node->fd_in);
			node->fd_in = new_fd;
		}
		redir = redir->next;
	}
	return (0);
}
