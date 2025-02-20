/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:57:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 11:50:57 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection.h"

t_redirection	*create_redirection(char *file, int type)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->file = file;
	redir->type = type;
	return (redir);
}

void	add_redirection(t_list **redirections, t_redirection *redir)
{
	ft_lstadd_back(redirections, ft_lstnew(redir));
}

void	free_redirection(void *redir)
{
	t_redirection	*red;

	red = (t_redirection *)redir;
	free(red->file);
	free(red);
}
