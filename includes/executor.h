/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/22 18:44:13 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"
# include "builtin.h"

typedef struct s_pipes		t_pipes;
typedef struct s_executor	t_executor;
struct s_executor
{
	t_list				*cmds;
	const t_builtins	*builtins_list;
	t_pipes				*pipes;
	int					original_stdin;
	int					original_stdout;
	int					(*execute)(t_executor *, t_error_handler *, t_list *);
};

struct s_pipes
{
	int		prev_pipe[2];
	int		next_pipe[2];
};

t_executor	*create_executor(void);
void		free_executor(t_executor *executor);
void		repair_std_io(t_executor *self);

#endif