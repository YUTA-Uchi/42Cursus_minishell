/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:57:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/15 18:10:25 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"
#include "signals.h"

extern volatile sig_atomic_t	g_signal;

t_list	*create_redirection(char *file, t_redir_type type)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->type = type;
	redir->fd = -1;
	return (ft_lstnew(redir));
}

void	add_redirection(t_list **redirections, t_redirection *redir)
{
	ft_lstadd_back(redirections, ft_lstnew(redir));
}

void	free_redirection(void *redir)
{
	t_redirection	*redir_content;

	redir_content = (t_redirection *)redir;
	free(redir_content->file);
	free(redir_content);
}

bool	set_heredoc(t_redirection *redir_content)
{
	char	*delimiter;
	int		here_doc_pipe[2];
	pid_t	pid;
	int		status;
	char	*line;

	delimiter = ft_strdup(redir_content->file);
	if (!delimiter)
		exit(EXIT_FAILURE);
	if (pipe(here_doc_pipe) == -1)
		return (print_strerror("pipe"), false);
	pid = fork();
	if (pid == -1)
	{
		free(delimiter);
		close(here_doc_pipe[0]);
		close(here_doc_pipe[1]);
		return (print_strerror("fork"), false);
	}
	if (pid == 0)
	{
		// ft_printf(STDERR_FILENO, "signalheredoc before: %d\n", g_signal);
		close(here_doc_pipe[0]);
		if (!set_heredoc_signal_handler())
			exit(EXIT_FAILURE);
		while (true)
		{
			line = readline("heredoc> ");
			if (!line || g_signal == SIGINT)
			{
				free(delimiter);
				close(here_doc_pipe[1]);
				// ft_printf(STDERR_FILENO, "signalheredocing: %d\n", g_signal);
				if (g_signal == SIGINT)
					exit(130);
				exit(EXIT_SUCCESS);
			}
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			{
				free(line);
				free(delimiter);
				close(here_doc_pipe[1]);
				exit(EXIT_SUCCESS);
			}
			ft_printf(here_doc_pipe[1], "%s\n", line);
			free(line);
		}
	}
	free(delimiter);
	close(here_doc_pipe[1]);
	// ft_printf(STDERR_FILENO, "signalheredoc after: %d\n", g_signal);
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		close(here_doc_pipe[0]);
		return (print_strerror("waitpid"), false);
	}
	if (WIFSIGNALED(status))
	{
		g_signal = WTERMSIG(status);
		close(here_doc_pipe[0]);
		return (false);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		if (WEXITSTATUS(status) == 130)
			g_signal = SIGINT;
		close(here_doc_pipe[0]);
		return (false);
	}
	redir_content->fd = here_doc_pipe[0];
	return (true);
}

bool	open_redirections(t_list *redir_list)
{
	t_list			*current_redir;
	t_redirection	*redir_content;

	current_redir = redir_list;
	while (current_redir)
	{
		redir_content = (t_redirection *)(current_redir->content);
		if (redir_content->type == REDIR_IN)
			redir_content->fd = open(redir_content->file, O_RDONLY);
		else if (redir_content->type == REDIR_OUT)
			redir_content->fd = open(redir_content->file, O_WRONLY \
									| O_CREAT | O_TRUNC, 0644);
		else if (redir_content->type == REDIR_APPEND)
			redir_content->fd = open(redir_content->file, O_WRONLY \
									| O_CREAT | O_APPEND, 0644);
		else if (redir_content->type == REDIR_HEREDOC)
		{
			if (!set_heredoc(redir_content))
				return (false);
		}
		if (redir_content->fd == -1)
			return (print_strerror(redir_content->file), false);
		current_redir = current_redir->next;
	}
	return (true);
}
