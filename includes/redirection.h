/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:13 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 13:37:58 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "minishell.h"
# include "error_handler.h"

typedef enum e_redir_type
{
	REDIR_IN = 0,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redirection	t_redirection;
struct s_redirection
{
	t_redir_type	type;
	char			*file;
	int				fd;
};

t_list	*create_redirection(char *file, t_redir_type type);
void	free_redirection(void *redir);
bool	open_redirections(t_list *redir_list);
bool	set_heredoc(t_redirection *redir_content);
#endif