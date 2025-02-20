/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 16:31:33 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"
# include "builtin.h"

typedef struct s_executor	t_executor;
struct s_executor
{
	t_list				*cmds;
	const t_builtins	*builtins_list;
	int					(*excute)(t_executor *, t_error_handler *);
};

t_executor	*create_executor(void);
void		free_executor(t_executor *executor);
#endif