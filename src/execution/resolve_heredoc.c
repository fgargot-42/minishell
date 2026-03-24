/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 20:49:51 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/24 21:21:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <sys/wait.h>

static int	heredoc_event_hook(void)
{
	if (g_signal)
		rl_done = 1;
	return (0);
}

static void	heredoc_sigint_handler(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

static void	read_heredoc_lines(char *delimiter, int pipe_fd)
{
	char				*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0 || g_signal)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd);
		ft_putstr_fd("\n", pipe_fd);
		free(line);
	}
}

static void	read_heredoc(int *pipe_fd, char **delimiter, t_list *envs,
				t_ctx *ctx)
{
	struct sigaction	sa;
	int					i;

	i = 3;
	while (i < 1024)
	{
		if (i != pipe_fd[1])
			close(i);
		i++;
	}
	g_signal = 0;
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	rl_event_hook = heredoc_event_hook;
	close(pipe_fd[0]);
	read_heredoc_lines(*delimiter, pipe_fd[1]);
	close(pipe_fd[1]);
	free_tree(ctx->cmd_tree);
	ft_lstclear(&envs, env_free);
	free_string_array(delimiter);
	if (g_signal)
		exit(130);
	exit(0);
}

int	handle_heredoc(char **delimiter, t_list *envs, t_ctx *ctx)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid;

	status = 0;
	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
		read_heredoc(pipe_fd, delimiter, envs, ctx);
	close(pipe_fd[1]);
	if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			ctx->error_code = 130;
		else
			return (pipe_fd[0]);
	}
	close(pipe_fd[0]);
	return (-1);
}
