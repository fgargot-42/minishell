/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:04:18 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/22 00:48:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>

static void	read_heredoc(int *pipe_fd, char *delimiter)
{
	char	*line;

	close(pipe_fd[0]);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline(">");
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

static int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

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
		read_heredoc(pipe_fd, delimiter);
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	return (pipe_fd[0]);
}

static int	open_redir(t_redir **redir, t_list *envs, t_ctx *ctx)
{
	int		new_fd;
	char	*tmp;

	expand_var(&(*redir)->file, envs, ctx);
	tmp = remove_quotes((*redir)->file);
	free((*redir)->file);
	(*redir)->file = tmp;
	if ((*redir)->type == TOKEN_REDIR_IN)
		new_fd = file_open_read((*redir)->file, ctx);
	else if ((*redir)->type == TOKEN_REDIR_OUT)
		new_fd = file_open_write((*redir)->file, ctx);
	else if ((*redir)->type == TOKEN_APPEND)
		new_fd = file_open_append((*redir)->file, ctx);
	else if ((*redir)->type == TOKEN_HEREDOC)
		new_fd = handle_heredoc((*redir)->file);
	else
		new_fd = -1;
	return (new_fd);
}

static void	update_node_fds(t_node *node, t_redir *redir, int fd)
{
	if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
	{
		if (node->fd_out != STDOUT_FILENO)
			close(node->fd_out);
		node->fd_out = fd;
	}
	else
	{
		if (node->fd_in != STDIN_FILENO)
			close(node->fd_in);
		node->fd_in = fd;
	}
}

int	resolve_redirs(t_node *node, t_list *envs, t_ctx *ctx)
{
	t_redir	*redir;
	int		new_fd;

	if (!node)
		return (0);
	redir = node->redirs;
	while (redir)
	{
		new_fd = open_redir(&redir, envs, ctx);
		if (new_fd == -1)
		{
			cleanup_node_fds(node);
			return (1);
		}
		update_node_fds(node, redir, new_fd);
		redir = redir->next;
	}
	return (0);
}
