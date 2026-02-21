/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:52:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/21 19:54:18 by fgargot          ###   ########.fr       */
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

typedef struct s_prompt_parts
{
	char	*errcode;
	char	*icon;
	char	*name;
	char	*cwd;
	char	*sep;
	char	*color;
}			t_prompt_parts;

typedef struct s_ctx
{
	int	error_code;
}		t_ctx;

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

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_redir			*redirs;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*cmd; // set as null si pas fin de branche
}	t_node;

typedef struct s_cmd
{
	char			**args; // ["ls", "-la", NULL]
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
	struct s_token	*next;
}	t_token;

// lexer.c
t_token		*lexer(char *input);
char		*extract_word(t_lexer *lexer);

// lexer_token.c

t_token		*get_next_token(t_lexer *lexer);

// lexer_utils.c

int			is_redir_token(t_token *token);
int			is_operator_token(t_token *token);
void		free_tokens(t_token *tokens);
char		current_char(t_lexer *lexer);
int			is_special(char c);

// lexer_syntax.c
int			lexer_has_syntax_error(t_token *lexer, t_ctx *ctx);

// tree_parser.c 
t_node		*parse_tree(t_token *tokens);
t_node		*parse_and_or(t_token **tokens);
t_node		*parse_pipe(t_token **tokens);
t_node		*parse_primary(t_token **tokens);
t_cmd		*parse_command(t_token **tokens, t_redir **redir);

// parser_utils.c
void		free_tree(t_node *root);
size_t		count_args(t_token *tokens);
void		handle_word_token(t_cmd *cmd, t_token **tokens, int *i);
int			is_stop_token(t_token *token);
void		init_cmd(t_cmd **cmd, size_t count);

// nodes.c
t_node		*create_node(t_node_type type, t_node *left, t_node *right,
				t_redir *redir);
t_node		*create_cmd_node(t_cmd *cmd, t_redir *redir);

// redir.c
void		cleanup_node_fds(t_node *node);
int			is_redirection(t_token_type type);
void		add_redirection(t_redir **redir, t_token **tokens);
int			resolve_redirs(t_node *node, t_list *envs, t_ctx *ctx);

// execution.c
int			exec_command(t_node *node, t_list **envs, t_ctx *ctx);
char		*find_in_path(char *cmd);
void		exit_fork_clean(t_node *node, char **char_envs, char *path);

// exec_tree
void		exec(t_node *root, t_list **envs, t_ctx *ctx);
void		propagate_redirs(t_node *node);

// exec_pipeline.c
int			exec_pipeline(t_node *node, t_list **envs, t_ctx *ctx);

// exec_path.c

char		*get_command_path(char *cmd, t_list *env);

// expander.c

void		expand_var(char **input, t_list *envs, t_ctx *ctx);
void		expand_cmd_args(t_node *node, t_list **envs, t_ctx *ctx);

// expander_split.c

int			split_add(char ***split_str, char *new_string, int pos);

// expander_replace.c

size_t		replace_env(char **input, t_env *env, char *key, size_t pos);
size_t		replace_errorcode_env(char **input, size_t pos, t_ctx *ctx);

// expander_utils.c

char		*extract_var_name(char *input, size_t start_pos,
				size_t *end_pos);
t_env		*get_env(t_list *envs, char *key);
int			is_special_dollar(char *input, size_t i, int *open_quotes);
int			count_words(char *s);
void		update_open_quotes(char c, int *quotes);

// builtin.c
typedef int	(*t_builtin_func)(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			is_builtin(t_cmd *cmd);
int			call_builtin(t_node *node, t_list **envs, t_ctx *ctx);
void		builtin_export_print(t_list **envs);

// env.c

t_env		*new_env(char *env_line);
t_list		*generate_env(char **env);
void		print_env_export(t_list *env);
const char	**reconstruct_envs(t_list *envs);

// env_utils.c
t_list		*get_env_node_by_key(t_list *env_list, char *key);

// builtins;
int			builtin_echo(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			builtin_cd(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			builtin_pwd(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			builtin_export(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			builtin_unset(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			builtin_env(t_cmd *cmd, t_list **envs, t_ctx *ctx);
int			builtin_exit(t_cmd *cmd, t_list **envs, t_ctx *ctx);

void		remove_args_quotes(char **args);

// export_utils
void		free_env(t_list *env_list);
void		env_free(void *content);
void		free_string_array(char **array);
int			add_quoted_arg_value(char **arg, char **env);
int			add_env(char **env, t_list **env_list);

// file utils
int			file_open_read(char *filepath, t_ctx *ctx);
int			file_open_write(char *filepath, t_ctx *ctx);
int			file_open_append(char *filepath, t_ctx *ctx);

// string utils
char		*remove_quotes(char *str);
char		*ft_strjoin_chr(char *str1, char *str2, char sep);
char		*ft_strjoin_all_chr(char **str_array, char sep);

// wildcards
int			has_wildcards(char *str);
char		**expand_wildcards(char *pattern);

#endif	//MINISHELL_H
