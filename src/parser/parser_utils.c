/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 21:47:34 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/20 19:38:17 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	free_tree(t_node *tree)
{
	t_redir	*redir;
	t_redir	*next;

	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	if (tree->cmd)
	{
		free_string_array(tree->cmd->args);
		redir = tree->cmd->redirs;
		while (redir)
		{
			next = redir->next;
			free(redir->file);
			free(redir);
			redir = next;
		}
		if (tree->cmd->envs)
			ft_lstclear(&tree->cmd->envs, env_free);
		free(tree->cmd);
	}
	free(tree);
}

size_t	count_args(t_token *tokens)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		else if (is_redir_token(tmp))
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (count);
}

void	init_cmd(t_cmd **cmd, size_t count)
{
	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!*cmd)
		return ;
	(*cmd)->args = malloc(sizeof(char *) * (count + 1));
	if (!(*cmd)->args)
	{
		free(*cmd);
		return ;
	}
	(*cmd)->redirs = NULL;
	(*cmd)->envs = NULL;
}

int	is_stop_token(t_token *token)
{
	return (token->type == TOKEN_PIPE || token->type == TOKEN_AND
		|| token->type == TOKEN_OR || token->type == TOKEN_RPAREN);
}

void	handle_word_token(t_cmd *cmd, t_token **tokens, int *i)
{
	cmd->args[*i] = ft_strdup((*tokens)->value);
	if (!cmd->args[*i])
	{
		cmd->args[*i] = ft_strdup("");
		if (!cmd->args[*i])
			return ;
	}
	(*i)++;
	*tokens = (*tokens)->next;
}
