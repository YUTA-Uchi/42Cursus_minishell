/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/23 17:12:08 by yuuchiya         ###   ########.fr       */
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

// void	print_cmd(t_list *cmds)
// {
// 	int		i;
// 	t_cmd	*cmd_content;
// 	t_list	*redir_list;
// 	t_list	*cmd_list;

// 	cmd_list = cmds;
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

static void	display_signal_message(int signal, bool *sigint_displayed \
									, bool *sigquit_displayed)
{
	if (signal == SIGINT && !(*sigint_displayed))
	{
		ft_printf(STDERR_FILENO, "\n");
		*sigint_displayed = true;
	}
	else if (signal == SIGQUIT && !(*sigquit_displayed))
	{
		ft_printf(STDERR_FILENO, "Quit (core dumped)\n");
		*sigquit_displayed = true;
	}
}

static int	handle_exit_status(int status, bool *sigint_displayed \
							, bool *sigquit_displayed)
{
	int	signal;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		display_signal_message(signal, sigint_displayed, sigquit_displayed);
		return (signal + E_SIGTERM);
	}
	return (E_GENERAL_ERR);
}

static int	wait_single_command(t_cmd *cmd_content, bool *sigint_displayed \
								, bool *sigquit_displayed)
{
	int		status;
	pid_t	wait_result;

	while (1)
	{
		wait_result = waitpid(cmd_content->pid, &status, 0);
		if (wait_result != -1 || errno != EINTR)
			break ;
	}
	if (wait_result == -1)
		return (E_GENERAL_ERR);
	return (handle_exit_status(status, sigint_displayed, sigquit_displayed));
}

int	wait_all_children(t_list *cmd_list)
{
	t_list	*current;
	bool	sigint_displayed;
	bool	sigquit_displayed;
	int		last_status;
	t_cmd	*cmd_content;

	sigint_displayed = false;
	sigquit_displayed = false;
	last_status = 0;
	current = cmd_list;
	while (current)
	{
		cmd_content = (t_cmd *)(current->content);
		last_status = wait_single_command(cmd_content, &sigint_displayed,
				&sigquit_displayed);
		current = current->next;
	}
	if (errno != 0 && errno != ECHILD && errno != EINTR)
	{
		ft_printf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		return (E_GENERAL_ERR);
	}
	return (last_status);
}





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
	if (close(STDIN_FILENO) == -1)
		return (print_strerror("close"), false);
	if (dup2(self->original_stdin, STDIN_FILENO) == -1)
		return (print_strerror("dup2"), false);
	if (close(self->original_stdin) == -1)
		return (print_strerror("close"), false);
	if (close(STDOUT_FILENO) == -1)
		return (print_strerror("close"), false);
	if (dup2(self->original_stdout, STDOUT_FILENO) == -1)
		return (print_strerror("dup2"), false);
	if (close(self->original_stdout) == -1)
		return (print_strerror("close"), false);
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
		close(executor->original_stdin);
	if (is_fd_open(executor->original_stdout))
		close(executor->original_stdout);
	free(executor);
}
