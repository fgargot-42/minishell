/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:41:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/11 21:24:29 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
void cleanup_node_fds(t_node *node)
{
	if (!node)
		return ;

	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		node->fd_in = STDIN_FILENO;
	}

	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		node->fd_out = STDOUT_FILENO;
	}
}
static int handle_heredoc(char *delimiter)
{
	int pipe_fd[2];
	char *line;
	pid_t pid;
	if (pipe(pipe_fd) == -1)
		return (-1);

	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}

	if (pid == 0)
	{
		close(pipe_fd[0]);

		while (1)
		{
			line = readline(">");
			if (!line)
				break;

			if (strcmp(line,delimiter) == 0)
			{
				free(line);
				break;
			}
			write(pipe_fd[1], line, strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		close(pipe_fd[1]);
		exit(0);
	}

	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	return (pipe_fd[0]);
}
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
	//if ((*tokens)->quote != QUOTE_SINGLE)
	//	expand_var(&redir->file, envs, ctx);
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

int	resolve_redirs(t_node *node, t_list *envs, t_ctx *ctx)
{
	t_redir	*redir;
	int		new_fd;

	if (!node->cmd)
		return (1);
	redir = node->cmd->redirs;
	while (redir)
	{
		expand_var_redir(&redir->file, envs, ctx);
		new_fd = 0;
		if (redir->type == TOKEN_REDIR_IN)
			new_fd = open(redir->file, O_RDONLY);
		if (redir->type == TOKEN_REDIR_OUT)
			new_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->type == TOKEN_APPEND)
			new_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (redir->type == TOKEN_HEREDOC)
			new_fd = handle_heredoc(redir->file);
		if (new_fd == -1)
		{
			fprintf(stderr, "minishell: %s: No such file or directory\n", redir->file);
			cleanup_node_fds(node);
			return (1);
		}
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
