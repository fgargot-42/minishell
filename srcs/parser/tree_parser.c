#include "minishell.h"
#include <string.h>

t_node	*parse_tree(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	return (parse_or(&current));
}

t_node	*parse_or(t_token **tokens)
{
	t_node	*left;
	t_node	*right;

	left = parse_and(tokens); // get la gauche en premer
	while (*tokens && (*tokens)->type == TOKEN_OR)
	{
		*tokens = (*tokens)->next; // skip les ||
		right = parse_and(tokens);
		;
		left = create_node(NODE_OR, left, right);
	}
	return (left);
}

t_node	*parse_and(t_token **tokens)
{
	t_node	*left;
	t_node	*right;

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
	t_node	*left;

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
	t_node	*node;
	t_cmd	*cmd;

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
static size_t	count_args(t_token *tokens)
{
	size_t count;
	t_token *tmp;
	
	count = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		if (is_redirection(tmp->type))
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (count);
}
static void init_cmd(t_cmd **cmd, size_t count)
{
	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	(*cmd)->args = malloc(sizeof(char *) * (count + 1));
	(*cmd)->quote_type = malloc(sizeof(t_quote_type *) * (count + 1));
}

static int is_stop_token(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND
			|| type == TOKEN_OR || type == TOKEN_RPAREN);
}

static void handle_word_token(t_cmd *cmd, t_token **tokens, int *i)
{
	cmd->args[*i] = strdup((*tokens)->value);
	cmd->quote_type[*i] = (*tokens)->quote;
	(*i)++;
	*tokens = (*tokens)->next;
}

t_cmd	*parse_command(t_token **tokens)
{
	int			i;
	t_cmd		*cmd;
	size_t		count;

	cmd = NULL;
	count = count_args(*tokens);
	init_cmd(&cmd, count);
	cmd->redirs = NULL;
	i = 0;
	while (*tokens)
	{
		if (is_stop_token((*tokens)->type))
			break ;
		if (is_redirection((*tokens)->type))
		{
			add_redirection(cmd, tokens);
			continue ;
		}
		if ((*tokens)->type == TOKEN_WORD)
			handle_word_token(cmd, tokens, &i);
		else
			*tokens = (*tokens)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

void	print_str_list(char **str_list)
{
	int	i;

	i = 0;
	fprintf(stderr,
		CYAN "\n═══════════════════════════ ARGS ════════════════════════════\n" RESET);
	fprintf(stderr, BLUE "⟩ " CYAN "[" RESET);
	while (str_list[i])
	{
		fprintf(stderr, MAGENTA "%s" RESET, str_list[i]);
		i++;
		if (str_list[i])
			fprintf(stderr, BLUE ", " RESET);
		else
			fprintf(stderr, BLUE ", " RESET);
	}
	fprintf(stderr, RED "NULL" CYAN "]\n" RESET);
}


void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_tree(t_node *tree)
{
	t_redir *redir;
	t_redir *next;
	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	if (tree->cmd)
	{
		free_string_array(tree->cmd->args);
		free(tree->cmd->quote_type);
		redir = tree->cmd->redirs;
		while (redir)
		{
			next = redir->next;
			free(redir->file);
			free(redir);
			redir = next;
		}
		free(tree->cmd);
	}
	free(tree);
}
