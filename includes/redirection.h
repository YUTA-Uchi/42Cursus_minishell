/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:13 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 11:45:57 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

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
	int				from_fd;
	int				to_fd;
	t_redirection	*next;
};

t_redirection	*create_redirection(char *file, int from_fd, int to_fd, t_redir_type type);
void			free_redirection(t_redirection *redir);
#endif