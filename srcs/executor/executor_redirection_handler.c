/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirection_handler.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:52:12 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 14:48:29 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool	set_redirection_in(t_redirection *redir)
{
	int	fd;

	if (close(STDIN_FILENO) == -1)
		return (print_strerror("close"), false);
	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
		return (print_strerror(redir->file), false);
	if (fd != STDIN_FILENO)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (print_strerror("dup2"), false);
		if (close(fd) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}

static bool	set_redirection_out(t_redirection *redir)
{
	int	fd;

	if (close(STDOUT_FILENO) == -1)
		return (print_strerror("close"), false);
	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (print_strerror(redir->file), false);
	if (fd != STDOUT_FILENO)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (print_strerror("dup2"), false);
		if (close(fd) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}

static bool	set_redirection_append(t_redirection *redir)
{
	int	fd;

	if (close(STDOUT_FILENO) == -1)
		return (print_strerror("close"), false);
	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (print_strerror(redir->file), false);
	if (fd != STDOUT_FILENO)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (print_strerror("dup2"), false);
		if (close(fd) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}

bool	set_redirections(t_list *current_cmd)
{
	t_cmd			*cmd_content;
	t_list			*redir_list;
	t_redirection	*redir;

	cmd_content = (t_cmd *)(current_cmd->content);
	redir_list = cmd_content->redirections;
	while (redir_list)
	{
		redir = (t_redirection *)(redir_list->content);
		if (redir->type == REDIR_IN)
		{
			if (!set_redirection_in(redir))
				return (false);
		}
		else if (redir->type == REDIR_OUT)
		{
			if (!set_redirection_out(redir))
				return (false);
		}
		else if (redir->type == REDIR_APPEND)
		{
			if (!set_redirection_append(redir))
				return (false);
		}
		redir_list = redir_list->next;
	}
	return (true);
}
