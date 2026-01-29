/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:52:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/29 20:07:55 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include "libft.h"

// Color codes
# define RED     "\001\033[0;31m\002"
# define GREEN   "\001\033[0;32m\002"
# define YELLOW  "\001\033[0;33m\002"
# define BLUE    "\001\033[0;34m\002"
# define MAGENTA "\001\033[0;35m\002"
# define CYAN    "\001\033[0;36m\002"
# define RESET   "\001\033[0m\002"


typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env *next;
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
	char		**args; // ["ls", "-la", NULL]
	t_redir		*redirs;
	t_list		*envs;
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

// redir.c
void	print_redirs(t_redir *redirs);
int		is_redirection(t_token_type type);
void	add_redirection(t_cmd *cmd, t_token **tokens);

// execution.c
int		exec_command(t_cmd *cmd, t_list **envs);

// exec_tree
int		exec(t_node *root, t_list **envs);

// exec_pipeline.c
int		exec_pipeline(t_node *node, t_list **envs);
// builtin.c

typedef int	(*t_builtin_func)(t_cmd *cmd, t_list **envs);
int		is_builtin(t_cmd *cmd);
int		call_builtin(t_cmd *cmd, t_list **envs);

// env.c

t_list	*generate_env(char **env);
void	print_env_export(t_list *env);
const char	**reconstruct_envs(t_list *envs);

// builtins;
int		builtin_echo(t_cmd *cmd, t_list **envs);
int		builtin_cd(t_cmd *cmd, t_list **envs);
int		builtin_pwd(t_cmd *cmd, t_list **envs);
int		builtin_export(t_cmd *cmd, t_list **envs);
int		builtin_unset(t_cmd *cmd, t_list **envs);
int		builtin_env(t_cmd *cmd, t_list **envs);
int		builtin_exit(t_cmd *cmd, t_list **envs);

#endif	//MINISHELL_H
