/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:55:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 19:40:03 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"

extern volatile sig_atomic_t	g_signal;

bool	setup_io_redirections(t_executor *self, t_list *current_cmd \
								, t_shell_state *shell_state)
{
	if (!set_pipes(self, current_cmd, shell_state->error_handler))
		return (false);
	if (!set_redirections(current_cmd))
		return (false);
	return (true);
}

static bool	try_builtin_execution(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state, int *exit_code)
{
	t_cmd				*cmd_content;
	const t_builtins	*builtin;

	cmd_content = (t_cmd *)(current_cmd->content);
	builtin = lookup_builtin(cmd_content->cmd_name, self->builtins_list);
	if (builtin->name)
	{
		*exit_code = builtin->func(self, current_cmd, shell_state);
		return (true);
	}
	return (false);
}

static void	exit_with_command_error(char *cmd_name \
						, t_executor *self, t_shell_state *shell_state)
{
	if (errno == EACCES)
		ft_printf(STDERR_FILENO, "%s: %s\n", cmd_name, COMMAND_NOT_FOUND);
	else
		print_strerror(cmd_name);
	terminate_shell(self, shell_state, get_err_status());
}

void	execute_child_process(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state)
{
	t_cmd	*cmd_content;
	int		exit_code;
	int		exec_ret;

	if (!setup_io_redirections(self, current_cmd, shell_state))
		terminate_shell(self, shell_state, errno);
	cmd_content = (t_cmd *)(current_cmd->content);
	if (try_builtin_execution(self, current_cmd, shell_state, &exit_code))
		exit(exit_code);
	if (ft_strchr(cmd_content->cmd_name, '/'))
		exec_ret = execve_in_absolute_path(cmd_content, shell_state->env_list);
	else
		exec_ret = ft_execvp(cmd_content, shell_state->env_list);
	if (exec_ret == -1)
		exit_with_command_error(cmd_content->cmd_name \
							, self, shell_state);
	ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
				, COMMAND_NOT_FOUND);
	terminate_shell(self, shell_state, get_err_status());
}

bool	parent_process(t_pipes *pipes, t_list *redir_list)
{
	t_redirection	*redir_content;

	if (pipes->prev_pipe[0] > -1 && is_fd_open(pipes->prev_pipe[0]))
	{
		if (!safe_close(pipes->prev_pipe[0]))
			return (false);
	}
	if (pipes->prev_pipe[1] > -1 && is_fd_open(pipes->prev_pipe[1]))
	{
		if (!safe_close(pipes->prev_pipe[1]))
			return (false);
	}
	while (redir_list)
	{
		redir_content = (t_redirection *)(redir_list->content);
		if (is_fd_open(redir_content->fd))
		{
			if (!safe_close(redir_content->fd))
				return (false);
		}
		redir_list = redir_list->next;
	}
	return (true);
}
