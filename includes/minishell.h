/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:52:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/01/27 15:00:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>

// Color codes
# define RED     "\033[0;31m"
# define GREEN   "\033[0;32m"
# define YELLOW  "\033[0;33m"
# define BLUE    "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN    "\033[0;36m"
# define RESET   "\033[0m"

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
t_cmd	*parser(t_token *tokens);
void	print_str_list(char **str_list);

// redir.c
void	print_redirs(t_redir *redirs);
int		is_redirection(t_token_type type);
void	add_redirection(t_cmd *cmd, t_token **tokens);

#endif	//MINISHELL_H
