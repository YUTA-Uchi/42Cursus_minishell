/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front_desk.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:04 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 17:26:28 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRONT_DESK_H
# define FRONT_DESK_H


# include "minishell.h"
# include "parser.h"
# include "executor.h"
# include "error_handler.h"

# define PROMPT "minishell$ "

typedef struct s_front_desk	t_front_desk;

struct s_front_desk
{
	t_parser		*parser;
	t_executor		*executor;
	t_error_handler	*error_handler;
	char			*(*listen)(t_front_desk *, const char *);
};

t_front_desk	*create_front_desk(void);
void			free_front_desk(t_front_desk *front_desk);

#endif
