/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_flow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:07:26 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/23 17:29:09 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"
#include "signals.h"

bool	is_single_builtin(t_executor *self)
{
	t_cmd	*cmd_content;

	if (!self->cmds || self->cmds->next)
		return (false);
	cmd_content = (t_cmd *)(self->cmds->content);
	if (!cmd_content->cmd_name)
		return (false);
	return (lookup_builtin(cmd_content->cmd_name \
			, self->builtins_list)->name != NULL);
}

int	execute_single_builtin(t_executor *self, t_shell_state *shell_state)
{
	t_list	*cmd;
	t_cmd	*cmd_content;

	cmd = self->cmds;
	cmd_content = (t_cmd *)(cmd->content);
	if (!open_redirections(cmd_content->redirections))
		return (get_err_status());
	if (!set_redirections(cmd))
		return (get_err_status());
	return (lookup_builtin(cmd_content->cmd_name \
		, self->builtins_list)->func(self, cmd, shell_state));
}

static bool	setup_command_execution(t_executor *self, t_list *cmd)
{
	t_cmd	*cmd_content;

	if (!open_redirections(((t_cmd *)(cmd->content))->redirections))
		return (false);
	self->pipes->prev_pipe[0] = self->pipes->next_pipe[0];
	self->pipes->prev_pipe[1] = self->pipes->next_pipe[1];
	if (cmd->next && pipe(self->pipes->next_pipe) == -1)
		return (print_strerror("pipe"), false);
	cmd_content = (t_cmd *)(cmd->content);
	if (!cmd_content->cmd_name)
		return (false);
	return (true);
}

static bool	fork_and_execute_command(t_executor *self, t_list *cmd \
	, t_shell_state *shell_state)
{
	t_cmd	*cmd_content;

	cmd_content = (t_cmd *)(cmd->content);
	cmd_content->pid = fork();
	if (cmd_content->pid == -1)
		return (print_strerror("fork"), false);
	if (cmd_content->pid == 0)
	{
		set_child_signal_handler();
		execute_child_process(self, cmd, shell_state);
	}
	return (parent_process(self->pipes \
		, ((t_cmd *)(cmd->content))->redirections));
}

int	execute_external_commands(t_executor *self \
	, t_shell_state *shell_state)
{
	t_list	*current_cmd;

	current_cmd = self->cmds;
	while (current_cmd)
	{
		if (!setup_command_execution(self, current_cmd))
			return (get_err_status());
		if (!fork_and_execute_command(self, current_cmd, shell_state))
			return (get_err_status());
		current_cmd = current_cmd->next;
	}
	return (wait_all_children(self->cmds));
}
