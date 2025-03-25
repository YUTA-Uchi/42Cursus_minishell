/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:57:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 13:35:30 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

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
	if (is_fd_open(redir_content->fd))
		safe_close(redir_content->fd);
	free(redir_content);
}

bool	open_redirections(t_list *redir_list, t_executor *executor \
						, t_shell_state *shell_state)
{
	t_list			*current_redir;
	t_redirection	*redir_content;

	current_redir = redir_list;
	while (current_redir)
	{
		redir_content = (t_redirection *)(current_redir->content);
		if (redir_content->type == REDIR_IN)
			redir_content->fd = safe_open(redir_content->file, O_RDONLY, 0644);
		else if (redir_content->type == REDIR_OUT)
			redir_content->fd = safe_open(redir_content->file, O_WRONLY \
									| O_CREAT | O_TRUNC, 0644);
		else if (redir_content->type == REDIR_APPEND)
			redir_content->fd = safe_open(redir_content->file, O_WRONLY \
									| O_CREAT | O_APPEND, 0644);
		else if (redir_content->type == REDIR_HEREDOC)
		{
			if (!set_heredoc(redir_content, executor, shell_state))
				return (false);
		}
		if (redir_content->fd == -1)
			return (false);
		current_redir = current_redir->next;
	}
	return (true);
}
