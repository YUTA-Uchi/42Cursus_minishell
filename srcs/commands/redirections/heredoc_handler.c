/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:53:30 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/23 16:54:49 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"
#include "signals.h"

extern volatile sig_atomic_t	g_signal;

static bool	setup_heredoc_pipe(int here_doc_pipe[2])
{
	if (pipe(here_doc_pipe) == -1)
		return (print_strerror("pipe"), false);
	return (true);
}

static void	process_heredoc_input(int pipe_write, char *delimiter)
{
	char	*line;

	while (true)
	{
		line = readline("heredoc> ");
		if (!line || g_signal == SIGINT)
		{
			free(line);
			free(delimiter);
			close(pipe_write);
			if (g_signal == SIGINT)
				exit(130);
			exit(EXIT_SUCCESS);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			free(delimiter);
			close(pipe_write);
			exit(EXIT_SUCCESS);
		}
		ft_printf(pipe_write, "%s\n", line);
		free(line);
	}
}

static void	run_heredoc_child(int here_doc_pipe[2], char *delimiter)
{
	close(here_doc_pipe[0]);
	if (!set_heredoc_signal_handler())
		exit(EXIT_FAILURE);
	process_heredoc_input(here_doc_pipe[1], delimiter);
}

static bool	wait_heredoc_child(pid_t pid, int pipe_read)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		close(pipe_read);
		return (print_strerror("waitpid"), false);
	}
	if (WIFSIGNALED(status))
	{
		g_signal = WTERMSIG(status);
		close(pipe_read);
		return (false);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		if (WEXITSTATUS(status) == 130)
			g_signal = SIGINT;
		close(pipe_read);
		return (false);
	}
	return (true);
}

bool	set_heredoc(t_redirection *redir_content)
{
	char	*delimiter;
	int		here_doc_pipe[2];
	pid_t	pid;

	delimiter = ft_strdup(redir_content->file);
	if (!delimiter)
		return (false);
	if (!setup_heredoc_pipe(here_doc_pipe))
	{
		free(delimiter);
		return (false);
	}
	pid = fork();
	if (pid == -1)
	{
		free(delimiter);
		close(here_doc_pipe[0]);
		close(here_doc_pipe[1]);
		return (print_strerror("fork"), false);
	}
	if (pid == 0)
		run_heredoc_child(here_doc_pipe, delimiter);
	free(delimiter);
	close(here_doc_pipe[1]);
	if (!wait_heredoc_child(pid, here_doc_pipe[0]))
		return (false);
	redir_content->fd = here_doc_pipe[0];
	return (true);
}