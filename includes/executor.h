/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/31 15:18:13 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"
# include "environment.h"
# include "shell_state.h"
# include "syscall_wrapper.h"

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
	int					(*execute)(t_executor *, t_shell_state *);
};

struct s_pipes
{
	int		prev_pipe[2];
	int		next_pipe[2];
};

struct s_builtins
{
	char	*name;
	int		(*func)(t_executor *, t_list *, t_shell_state *);
};

typedef bool				(*t_redirection_handler)(t_redirection *);

t_executor	*create_executor(void);
void		free_executor(t_executor *executor);
int			terminate_shell(t_executor *executor, t_shell_state *sh_state \
				, int status);
bool		is_single_builtin(t_executor *self);
int			execute_single_builtin(t_executor *self \
									, t_shell_state *shell_state);
int			execute_external_commands(t_executor *self \
									, t_shell_state *shell_state);
int			execve_in_absolute_path(t_cmd *cmd, t_list *env_list);
int			ft_execvp(t_cmd *cmd, t_list *env_list);
bool		repair_std_io(t_executor *self);
bool		set_redirections(t_list *current_cmd);
int			wait_all_children(t_list *cmd_list, t_shell_state *shell_state);
void		execute_child_process(t_executor *self, t_list *current_cmd \
								, t_shell_state *shell_state);
bool		parent_process(t_pipes *pipes, t_list *redir_list);

t_pipes		*create_pipes(void);
bool		set_pipes(t_executor *self, t_list *current_cmd);
void		free_pipes(t_pipes *pipes);

#endif