/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:57:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/11 14:24:55 by yuuchiya         ###   ########.fr       */
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

void	free_redirection(t_list **redir)
{
	ft_lstclear(redir, free);
}
