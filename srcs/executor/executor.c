/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 15:21:26 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"
#include "signals.h"

void	all_clear_exit(t_executor *executor, t_shell_state *sh_state, int status)
{
	if (executor)
		free_executor(executor);
	free_shell_state(sh_state);
	exit(status);
}

void	print_cmd(t_list *cmds)
{
	int		i;
	t_cmd	*cmd_content;
	t_list	*redir_list;
	t_list	*cmd_list;

	cmd_list = cmds;
	while (cmd_list)
	{
		cmd_content = (t_cmd *)(cmd_list->content);
		ft_printf(STDERR_FILENO, "cmd_name: %s\n", cmd_content->cmd_name);
		i = 0;
		while (cmd_content->args[i])
		{
			ft_printf(STDERR_FILENO, "args[%d]: %s\n", i, cmd_content->args[i]);
			i++;
		}
		i = 0;
		ft_printf(STDERR_FILENO, "redir:%p\n", (cmd_content->redirections));
		redir_list = cmd_content->redirections;
		while (redir_list)
		{
			ft_printf(STDERR_FILENO, "redirections[%d]: %s:%d\n", i, 
((t_redirection *)(redir_list->content))->file, ((t_redirection *)
(redir_list->content))->type);
			redir_list = redir_list->next;
			i++;
		}
		cmd_list = cmd_list->next;
	}
}

int	wait_all_children(t_list *cmd_list)
{
	int		status;
	int		last_status;
	t_list	*current_cmd;
	t_cmd	*cmd_content;
	pid_t	wait_result;
	bool	sigint_displayed;
	bool	sigquit_displayed;

	sigint_displayed = false;
	sigquit_displayed = false;
	current_cmd = cmd_list;
	while (current_cmd)
	{
		cmd_content = (t_cmd *)(current_cmd->content);
		wait_result = waitpid(cmd_content->pid, &status, 0);
		if (wait_result == -1 && errno == EINTR)
			continue ;
		if (wait_result != -1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				last_status = WTERMSIG(status) + E_SIGTERM;
				if (WTERMSIG(status) == SIGINT && !sigint_displayed)
				{
					ft_printf(STDERR_FILENO, "\n");
					sigint_displayed = true;
				}
				else if (WTERMSIG(status) == SIGQUIT && !sigquit_displayed)
				{
					ft_printf(STDERR_FILENO, "Quit (core dumped)\n");
					sigquit_displayed = true;
				}
			}
		}
		current_cmd = current_cmd->next;
	}
	if (errno != 0 && errno != ECHILD && errno != EINTR)
	{
		ft_printf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		last_status = E_GENERAL_ERR;
	}
	return (last_status);
}



int	execute(t_executor *self, t_shell_state *shell_state)
{
	t_cmd	*cmd_content;
	t_list	*head;
	int		status;

	// ft_printf(STDERR_FILENO, "signal execute start: %d\n", g_signal);
	set_exec_signal_handler();
	// print_cmd(self->cmds);
	head = self->cmds;
	cmd_content = (t_cmd *)(head->content);
	if (!head->next && cmd_content->cmd_name && lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
	{
		status = lookup_builtin(cmd_content->cmd_name, self->builtins_list)->func(self, head, shell_state);
	}
	else
	{
		while (head)
		{
			if (!open_redirections(((t_cmd *)(head->content))->redirections))
				return (set_interactive_signal_handler(), get_err_status());
			self->pipes->prev_pipe[0] = self->pipes->next_pipe[0];
			self->pipes->prev_pipe[1] = self->pipes->next_pipe[1];
			if (head->next)
			{
				if (pipe(self->pipes->next_pipe) == -1)
					return (print_strerror("pipe"), errno);
			}
			cmd_content = (t_cmd *)(head->content);
			if (!cmd_content->cmd_name)
				return (set_interactive_signal_handler(), E_GENERAL_ERR);
			cmd_content->pid = fork();
			if (cmd_content->pid == -1)
				return (print_strerror("fork"), errno);
			else if (cmd_content->pid == 0)
			{
				// ft_printf(STDERR_FILENO, "signal execute child: %d\n", g_signal);
				set_child_signal_handler();
				execute_child_process(self, head, shell_state);
			}
			if (!parent_process(self->pipes, ((t_cmd *)(head->content))->redirections))
				return (set_interactive_signal_handler(), get_err_status());
			head = head->next;
		}
		status = wait_all_children(self->cmds);
	}
	// ft_printf(STDERR_FILENO, "signal execute end: %d\n", g_signal);
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
