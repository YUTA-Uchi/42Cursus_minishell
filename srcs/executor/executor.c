/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/13 13:57:33 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"

void	all_clear_exit(t_executor *executor, t_shell_state *sh_state, int status)
{
	free_executor(executor);
	free_shell_state(sh_state);
	exit(status);
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

int	wait_all_children(t_list *cmd_list)
{
	int		status;
	int		last_status;
	t_list	*current_cmd;
	t_cmd	*cmd_content;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		cmd_content = (t_cmd *)(current_cmd->content);
		if (waitpid(cmd_content->pid, &status, 0) != -1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = WTERMSIG(status) + E_SIGTERM;
		}
		current_cmd = current_cmd->next;
	}
	if (errno != 0 && errno != ECHILD)
	{
		if (errno != EACCES)
			ft_printf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		last_status = E_GENERAL_ERR;
	}
	return (last_status);
}



int	execute(t_executor *self, t_shell_state *shell_state)
{
	t_cmd	*cmd_content;
	t_list	*head;

	head = self->cmds;
	//print_cmd(head);
	cmd_content = (t_cmd *)(head->content);
	if (!head->next)
	{
		if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
		{
			return (lookup_builtin(cmd_content->cmd_name, \
				self->builtins_list)->func(self, shell_state));
		}
	}
	while (head)
	{
		if (!open_redirections(((t_cmd *)(head->content))->redirections))
			return (get_err_status());
		self->pipes->prev_pipe[0] = self->pipes->next_pipe[0];
		self->pipes->prev_pipe[1] = self->pipes->next_pipe[1];
		if (head->next)
		{
			if (pipe(self->pipes->next_pipe) == -1)
				return (print_strerror("pipe"), errno);
		}
		cmd_content = (t_cmd *)(head->content);
		cmd_content->pid = fork();
		if (cmd_content->pid == -1)
			return (print_strerror("fork"), errno);
		else if (cmd_content->pid > 0)
		{
			if (!parent_process(self->pipes))
				return (get_err_status());
			head = head->next;
			continue ;
		}
		execute_child_process(self, head, shell_state);
	}
	return (wait_all_children(self->cmds));
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
	return (executor);
}

void	free_executor(t_executor *executor)
{
	if (executor->cmds)
		free_cmd_list(&executor->cmds);
	if (executor->pipes)
		free_pipes(executor->pipes);
	free(executor);
}
