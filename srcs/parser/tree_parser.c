#include "minishell.h"
#include <string.h>

t_node *parse_tree(t_token *tokens)
{
	t_token *current;
	current = tokens;
	return (parse_or(&current));
}

t_node *parse_or(t_token **tokens)
{
	t_node *left;
	t_node *right;
	left = parse_and(tokens); // get la gauche en premer
	while (*tokens && (*tokens)->type == TOKEN_OR)
	{
		*tokens = (*tokens)->next; // skip les ||
		right = parse_and(tokens);;
		left = create_node(NODE_OR, left, right);
	}
	return (left);
}

t_node *parse_and(t_token **tokens)
{
	t_node *left;
	t_node *right;
	left = parse_pipe(tokens);

	while (*tokens && (*tokens)->type == TOKEN_AND)
	{
		*tokens = (*tokens)->next; // skip les &&
		right = parse_pipe(tokens);
		left = create_node(NODE_AND, left, right);
	}
	return (left);
}

t_node	*parse_pipe(t_token **tokens)
{
	t_node	*root;
	t_node *left;
	left = parse_primary(tokens);

	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		root = create_node(NODE_PIPE, left, NULL);
		*tokens = (*tokens)->next; // skip les |
		root->right = parse_pipe(tokens);
	}
	else
		root = left;

	return (root);
}

t_node	*parse_primary(t_token **tokens)
{
	t_node *node;
	t_cmd *cmd;

	if (*tokens && (*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next; // skip (
		node = parse_or(tokens);

		// il faut une parenthese fermante )
		if (*tokens && (*tokens)->type == TOKEN_RPAREN)
			*tokens = (*tokens)->next; // skip )
		else
		{
			fprintf(stderr, "error missing )\n");
			return (NULL);
		}
		return (create_node(NODE_GROUP, node, NULL));
	}
	cmd = parse_command(tokens);
	if (!cmd)
		return (NULL);
	return (create_cmd_node(cmd));
}


static size_t	count_words(t_token *tokens)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp && tmp->type == TOKEN_WORD)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

t_cmd	*parse_command(t_token **tokens)
{
	t_cmd			*cmd;
	int				i;
	const size_t	count = count_words(*tokens);

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->args = malloc(sizeof(char *) * (count + 1));
	cmd->redirs = NULL;
	// parsing args
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		cmd->args[i] = strdup((*tokens)->value);
		i++;
		*tokens = (*tokens)->next;
	}
	cmd->args[i] = NULL;
	// parsings redirs
	while (*tokens && is_redirection((*tokens)->type))
		add_redirection(cmd, tokens);
	return (cmd);
}
