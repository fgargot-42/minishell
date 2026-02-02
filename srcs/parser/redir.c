/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:41:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/02 02:22:20 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

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
	printf(CYAN "\n══════════════════════════ REDIRS ═══════════════════════════\n" RESET);
	printf(BLUE "⟩ " CYAN "[" RESET);
	while (current)
	{
		printf("(" YELLOW "%s" RESET ": " MAGENTA "%s" RESET ")",
			get_type_name(current->type), current->file);
		current = current->next;
		if (current)
			printf(BLUE ", " RESET);
	}
	printf(CYAN "]\n" RESET);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
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
