/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 21:30:07 by mabarrer          #+#    #+#             */
/*   Updated: 2026/02/23 23:40:10 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

t_node	*parse_tree(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	return (parse_and_or(&current));
}

t_node	*parse_and_or(t_token **tokens)
{
	t_node			*left;
	t_node			*right;
	t_token_type	type;
	t_redir			*redir;

	redir = NULL;
	left = parse_pipe(tokens);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		type = (*tokens)->type;
		*tokens = (*tokens)->next;
		right = parse_pipe(tokens);
		while (is_redir_token(*tokens))
			add_redirection(&redir, tokens);
		if (type == TOKEN_AND)
			left = create_node(NODE_AND, left, right, redir);
		else
			left = create_node(NODE_OR, left, right, redir);
	}
	return (left);
}

t_node	*parse_pipe(t_token **tokens)
{
	t_node	*root;
	t_node	*left;

	left = parse_primary(tokens);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		root = create_node(NODE_PIPE, left, NULL, NULL);
		*tokens = (*tokens)->next;
		root->right = parse_pipe(tokens);
	}
	else
		root = left;
	return (root);
}

t_node	*parse_primary(t_token **tokens)
{
	t_node	*node;
	t_cmd	*cmd;
	t_redir	*redir;

	redir = NULL;
	while (is_redir_token(*tokens))
		add_redirection(&redir, tokens);
	if (*tokens && (*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = parse_and_or(tokens);
		if (*tokens && (*tokens)->type == TOKEN_RPAREN)
			*tokens = (*tokens)->next;
		else
		{
			ft_putstr_fd("error missing )\n", 2);
			return (NULL);
		}
		while (is_redir_token(*tokens))
			add_redirection(&redir, tokens);
		return (create_node(NODE_GROUP, node, NULL, redir));
	}
	cmd = parse_command(tokens, &redir);
	return (create_cmd_node(cmd, redir));
}

t_cmd	*parse_command(t_token **tokens, t_redir **redir)
{
	int		i;
	t_cmd	*cmd;
	size_t	count;

	count = count_args(*tokens);
	init_cmd(&cmd, count);
	if (!cmd)
		return (NULL);
	i = 0;
	while (*tokens)
	{
		if (is_stop_token(*tokens))
			break ;
		if (is_redir_token(*tokens))
			add_redirection(redir, tokens);
		else if ((*tokens)->type == TOKEN_WORD)
			handle_word_token(cmd, tokens, &i);
		else
			*tokens = (*tokens)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}
