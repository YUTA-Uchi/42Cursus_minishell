/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirection_handler.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:52:12 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/11 19:37:32 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool	set_redirection_in(t_redirection *redir)
{
	if (is_fd_open(redir->fd))
	{
		if (close(STDIN_FILENO) == -1)
			return (print_strerror("close"), false);
		if (dup2(redir->fd, STDIN_FILENO) == -1)
			return (print_strerror("dup2 ggg"), false);
		if (close(redir->fd) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}

static bool	set_redirection_out(t_redirection *redir)
{
	if (close(STDOUT_FILENO) == -1)
		return (print_strerror("close"), false);
	if (dup2(redir->fd, STDOUT_FILENO) == -1)
		return (print_strerror("dup2"), false);
	if (close(redir->fd) == -1)
		return (print_strerror("close"), false);
	return (true);
}

static bool	set_redirection_append(t_redirection *redir)
{
	if (close(STDOUT_FILENO) == -1)
		return (print_strerror("close"), false);
	if (dup2(redir->fd, STDOUT_FILENO) == -1)
		return (print_strerror("dup2"), false);
	if (close(redir->fd) == -1)
		return (print_strerror("close"), false);
	return (true);
}

static bool	set_redirection_heredoc(t_redirection *redir)
{
	// ft_printf(STDERR_FILENO, "heredoc:%d\n", redir->fd);
	if (is_fd_open(redir->fd))
	{
		if (close(STDIN_FILENO) == -1)
			return (print_strerror("close"), false);
		if (dup2(redir->fd, STDIN_FILENO) == -1)
			return (print_strerror("dup2 ggg"), false);
		if (close(redir->fd) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}

bool	set_redirections(t_list *current_cmd)
{
	const t_redirection_handler	handlers[4] = {\
		set_redirection_in, \
		set_redirection_out, \
		set_redirection_append, \
		set_redirection_heredoc
	};
	t_cmd						*cmd_content;
	t_list						*redir_list;
	t_redirection				*redir_content;

	cmd_content = (t_cmd *)(current_cmd->content);
	redir_list = cmd_content->redirections;
	while (redir_list)
	{
		redir_content = (t_redirection *)(redir_list->content);
		if (!handlers[redir_content->type](redir_content))
			return (false);
		redir_list = redir_list->next;
	}
	return (true);
}
