/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 15:14:14 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"
# include "environment.h"

typedef struct s_builtins	t_builtins;
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

struct s_builtins
{
	char	*name;
	int		(*func)(t_executor *, t_error_handler *, t_list *);
};

t_executor	*create_executor(void);
void		free_executor(t_executor *executor);
void		all_clear_exit(t_executor *executor, t_list *env_list \
						, t_error_handler *error_handler, int status);
bool		repair_std_io(t_executor *self);
bool		set_redirections(t_list *current_cmd);

void		execute_child_process(t_executor *self, t_list *env_list \
					, t_list *current_cmd, t_error_handler *error_handler);
bool		parent_process(t_pipes *pipes);

t_pipes		*create_pipes(void);
bool		set_pipes(t_executor *self, t_list *current_cmd \
					, t_error_handler *error_handler);
void		free_pipes(t_pipes *pipes);

#endif