/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 16:06:07 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"
#include "signals.h"

int	terminate_shell(t_executor *executor, t_shell_state *sh_state, int status)
{
	if (executor)
		free_executor(executor);
	free_shell_state(sh_state);
	exit(status);
	return (0);
}

// void	print_cmd(t_executor *self)
// {
// 	int		i;
// 	t_cmd	*cmd_content;
// 	t_list	*redir_list;
// 	t_list	*cmd_list;

// 	cmd_list = self->cmds;
// 	while (cmd_list)
// 	{
// 		cmd_content = (t_cmd *)(cmd_list->content);
// 		ft_printf(STDERR_FILENO, "cmd_name: %s\n", cmd_content->cmd_name);
// 		i = 0;
// 		while (cmd_content->args[i])
// 		{
// 			ft_printf(STDERR_FILENO, "args[%d]: %s\n", i, cmd_content->args[i]);
// 			i++;
// 		}
// 		i = 0;
// 		ft_printf(STDERR_FILENO, "redir:%p\n", (cmd_content->redirections));
// 		redir_list = cmd_content->redirections;
// 		while (redir_list)
// 		{
// 			ft_printf(STDERR_FILENO, "redirections[%d]: %s:%d\n", i, 
// ((t_redirection *)(redir_list->content))->file, ((t_redirection *)
// (redir_list->content))->type);
// 			redir_list = redir_list->next;
// 			i++;
// 		}
// 		cmd_list = cmd_list->next;
// 	}
// }

int	execute(t_executor *self, t_shell_state *shell_state)
{
	int	status;

	set_exec_signal_handler();
	if (is_single_builtin(self))
		status = execute_single_builtin(self, shell_state);
	else
		status = execute_external_commands(self, shell_state);
	set_interactive_signal_handler();
	return (status);
}

bool	repair_std_io(t_executor *self)
{
	if (!safe_close(STDIN_FILENO))
		return (false);
	if (!safe_close(STDOUT_FILENO))
		return (false);
	if (!restore_std_fd(&self->original_stdin, &self->original_stdout))
		return (false);
	return (true);
}

t_executor	*create_executor(void)
{
	t_executor			*executor;

	executor = malloc(sizeof(t_executor));
	if (!executor)
		return (NULL);
	executor->cmds = NULL;
	executor->execute = execute;
	executor->builtins_list = create_builtins_list();
	executor->pipes = create_pipes();
	if (!executor->builtins_list || !executor->pipes)
		return (free_executor(executor), NULL);
	executor->original_stdin = dup(STDIN_FILENO);
	executor->original_stdout = dup(STDOUT_FILENO);
	if (executor->original_stdin == -1 || executor->original_stdout == -1)
		return (free_executor(executor), NULL);
	return (executor);
}

void	free_executor(t_executor *executor)
{
	if (executor->cmds)
		free_cmd_list(&executor->cmds);
	if (executor->pipes)
		free_pipes(executor->pipes);
	if (is_fd_open(executor->original_stdin))
		safe_close(executor->original_stdin);
	if (is_fd_open(executor->original_stdout))
		safe_close(executor->original_stdout);
	free(executor);
}
