/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:52:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/06 19:38:26 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include "libft.h"


#define DEBUG 0

// Color codes
# define RED     "\001\033[0;31m\002"
# define GREEN   "\001\033[0;32m\002"
# define YELLOW  "\001\033[0;33m\002"
# define BLUE    "\001\033[0;34m\002"
# define MAGENTA "\001\033[0;35m\002"
# define CYAN    "\001\033[0;36m\002"
# define RESET   "\001\033[0m\002"


# define PROMPT_OK    " ✨ "CYAN"> "RESET
# define PROMPT_127   " ❌ "RED"> "RESET
# define PROMPT_ERR   " ⚠️  "RED"> "RESET

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_prompt_parts
{
    char    *errcode;
    char    *icon;
    char    *name;
    char    *cwd;
    char    *sep;
    char    *color;
}   t_prompt_parts;


typedef struct s_ctx
{
	int error_code;
}	t_ctx;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_GROUP
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*cmd; // set as null si pas fin de branche
}	t_node;

/*

	echo "salut" && ls -l

		AND
		/ \
	  CMD  CMD
    
	cat file.txt | wc -l || (ls)
	
        OR
		/ \
	PIPE   cmd (ls)
	/   \
cmd(cat) cmd(wc)


	*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,			// |
	TOKEN_REDIR_IN,		// <
	TOKEN_REDIR_OUT,	// >
	TOKEN_APPEND,		// >>
	TOKEN_HEREDOC,		// <<
	TOKEN_AND,			// &&
	TOKEN_OR,			// ||
	TOKEN_LPAREN,		// (
	TOKEN_RPAREN,		// )
	TOKEN_EOF
}	t_token_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args; // ["ls", "-la", NULL]
	t_quote_type	*quote_type;
	t_redir			*redirs;
	t_list			*envs;
}	t_cmd;

typedef struct s_lexer
{
	char	*input;
	int		pos;
	int		len;
}	t_lexer;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote;
	struct s_token	*next;
}	t_token;

// lexer_utils.c
void	print_tokens(t_token *tokens);
void	free_tokens(t_token *tokens);

// lexer.c
t_token	*lexer(char *input);

// parser.c
//t_cmd	*parser(t_token *tokens);

// tree_parser.c le bon

t_node	*parse_tree(t_token *tokens);
t_node	*parse_or(t_token **tokens);
t_node	*parse_and(t_token **tokens);
t_node	*parse_pipe(t_token **tokens);
t_node	*parse_primary(t_token **tokens);
t_cmd	*parse_command(t_token **tokens);
void	print_str_list(char **str_list);
// nodes.c
t_node	*create_node(t_node_type type, t_node *left, t_node *right);
t_node	*create_cmd_node(t_cmd *cmd);
void	print_tree(t_node *node, int d);
void	free_tree(t_node *root);
// redir.c
void	print_redirs(t_redir *redirs);
int		is_redirection(t_token_type type);
void	add_redirection(t_cmd *cmd, t_token **tokens);
int		resolve_redirs(t_node *node, t_list *envs, t_ctx *ctx);

// execution.c
int		exec_command(t_node *node, t_list **envs, t_ctx *ctx);
char	*find_in_path(char *cmd);

// exec_tree
int		exec(t_node *root, t_list **envs, t_ctx *ctx);

// exec_pipeline.c
int		exec_pipeline(t_node *node, t_list **envs, t_ctx *ctx);

// expander.c

void	expand_var(char **input, t_list *envs, t_ctx *ctx);
// builtin.c

typedef int	(*t_builtin_func)(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		is_builtin(t_cmd *cmd);
int		call_builtin(t_node *node, t_list **envs, t_ctx *ctx);
void	builtin_export_print(t_list **envs);

// env.c

t_list	*new_env(char *env_line);
t_list	*generate_env(char **env);
void	print_env_export(t_list *env);
const char	**reconstruct_envs(t_list *envs);

// env_utils.c
t_list	*get_env_node_by_key(t_list *env_list, char *key);

// builtins;
int		builtin_echo(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		builtin_cd(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		builtin_pwd(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		builtin_export(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		builtin_unset(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		builtin_env(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int		builtin_exit(t_cmd *cmd, t_list **envs, t_ctx *ctx);

void	remove_args_quotes(char **args);

// export_utils
void	free_env(t_list *env_list);

void	env_free(void *content);
void	free_string_array(char **array);
#endif	//MINISHELL_H
