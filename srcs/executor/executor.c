/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/02 18:57:02 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"

void	all_clear_exit(t_executor *executor, t_error_handler *error_handler, t_list *env_list, int status)
{
	free_executor(executor);
	free_error_handler(error_handler);
	free_env_list(env_list);
	exit(status);
}


static char	*create_path(char *env_path, char *cmd)
{
	char	*path_part;
	char	*exec_path;

	path_part = ft_strjoin(env_path, "/");
	exec_path = ft_strjoin(path_part, cmd);
	free(path_part);
	return (exec_path);
}

int	execve_in_absolute_path(t_cmd *cmd, t_list *env_list)
{
	if (access(cmd->cmd_name, F_OK) == 0)
		return (execve(cmd->cmd_name, cmd->args, (char *const *)env_list_to_array(env_list)));
	return (ft_printf(STDERR_FILENO, "executor: %s\n", strerror(errno)), get_err_status());
}

int	ft_execvp(t_cmd *cmd, t_list *env_list)
{
	int				i;
	char			**env_pathes;
	char			*exec_path;

	env_pathes = ft_split(get_env_value(env_list, "PATH"), ':');
	if (env_pathes == NULL)
		return (ft_printf(STDERR_FILENO, "executor: %s\n", strerror(errno)), get_err_status());
	i = 0;
	while (env_pathes[i])
	{
		exec_path = create_path(env_pathes[i], cmd->cmd_name);
		if (access(exec_path, F_OK) == 0)
		{
			free_arr(env_pathes);
			return (execve(exec_path, cmd->args, (char *const *)env_list_to_array(env_list)));
		}
		free(exec_path);
		i++;
	}
	free_arr(env_pathes);
	return (ft_printf(STDERR_FILENO, "executor: %s\n", strerror(errno)), get_err_status());
}

void	print_cmd(t_list *cmds)
{
	int		i;
	t_cmd	*cmd_content;
	t_list	*cmd_list;

	cmd_list = cmds;
	while (cmd_list)
	{
		cmd_content = (t_cmd *)(cmd_list->content);
		ft_printf(STDOUT_FILENO, "cmd_name: %s\n", cmd_content->cmd_name);
		i = 0;
		while (cmd_content->args[i])
		{
			ft_printf(STDOUT_FILENO, "args[%d]: %s\n", i, cmd_content->args[i]);
			i++;
		}
		i = 0;
		ft_printf(STDOUT_FILENO, "redir:%p\n", (cmd_content->redirections));
		while (cmd_content->redirections)
		{
			ft_printf(STDOUT_FILENO, "redirections[%d]: %s:%d\n", i, ((t_redirection *)(cmd_content->redirections->content))->file, ((t_redirection *)(cmd_content->redirections->content))->type);
			cmd_content->redirections = cmd_content->redirections->next;
			i++;
		}
		cmd_list = cmd_list->next;
	}
}

t_pipes	*create_pipes(void)
{
	t_pipes	*pipes;

	pipes = malloc(sizeof(t_pipes));
	if (!pipes)
		return (NULL);
	pipes->prev_pipe[0] = -1;
	pipes->prev_pipe[1] = -1;
	pipes->next_pipe[0] = -1;
	pipes->next_pipe[1] = -1;
	return (pipes);
}

bool	set_pipes(t_executor *self, t_list *current_cmd, t_error_handler *error_handler)
{
	(void)error_handler;
	if (current_cmd != self->cmds)
	{
		if (close(STDIN_FILENO) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
		if (dup2(self->pipes->prev_pipe[0], STDIN_FILENO) == -1)
			return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
		if (close(self->pipes->prev_pipe[0]) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
		if (close(self->pipes->prev_pipe[1]) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	}
	if (current_cmd->next)
	{
		if (close(self->pipes->next_pipe[0]) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
		if (close(STDOUT_FILENO) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
		if (dup2(self->pipes->next_pipe[1], STDOUT_FILENO) == -1)
			return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
		if (close(self->pipes->next_pipe[1]) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	}
	return (true);
}

void	free_pipes(t_pipes *pipes)
{
	free(pipes);
}

bool	set_redirections(t_executor *self, t_error_handler *error_handler)
{
	t_cmd	*cmd_content;
	t_list	*redirections;
	int		fd;

	(void)error_handler;
	cmd_content = (t_cmd *)(self->cmds->content);
	redirections = cmd_content->redirections;
	while (redirections)
	{
		if (((t_redirection *)(redirections->content))->type == REDIR_IN)
		{
			if (close(STDIN_FILENO) == -1)
				return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
			fd = open(((t_redirection *)(redirections->content))->file, O_RDONLY);
			if (fd == -1)
				return (ft_printf(STDERR_FILENO, "open: %s\n", strerror(errno)), false);
			if (fd != STDIN_FILENO)
			{
				if (dup2(fd, STDIN_FILENO) == -1)
					return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
				if (close(fd) == -1)
					return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
			}
			redirections = redirections->next;
			continue ;
		}
		if (((t_redirection *)(redirections->content))->type == REDIR_OUT)
		{
			if (close(STDOUT_FILENO) == -1)
				return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
			fd = open(((t_redirection *)(redirections->content))->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (ft_printf(STDERR_FILENO, "open: %s\n", strerror(errno)), false);
			if (fd != STDOUT_FILENO)
			{
				if (dup2(fd, STDOUT_FILENO) == -1)
					return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
				if (close(fd) == -1)
					return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
			}
			redirections = redirections->next;
			continue ;
		}
		if (((t_redirection *)(redirections->content))->type == REDIR_APPEND)
		{
			if (close(STDOUT_FILENO) == -1)
				return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
			fd = open(((t_redirection *)(redirections->content))->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (ft_printf(STDERR_FILENO, "open: %s\n", strerror(errno)), false);
			if (fd != STDOUT_FILENO)
			{
				if (dup2(fd, STDOUT_FILENO) == -1)
					return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
				if (close(fd) == -1)
					return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
			}
			redirections = redirections->next;
			continue ;
		}
		redirections = redirections->next;
	}
	return (true);
}

void	execute_child_process(t_executor *self, t_error_handler *error_handler, t_list *env_list, t_list *current_cmd)
{
	t_cmd	*cmd_content;
	int		exec_ret;

	if (!set_pipes(self, current_cmd, error_handler))
		all_clear_exit(self, error_handler, env_list, 1);
	if (!set_redirections(self, error_handler))
		all_clear_exit(self, error_handler, env_list, 1);
	cmd_content = (t_cmd *)(current_cmd->content);
	if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
	{
		exit(lookup_builtin(cmd_content->cmd_name, \
			self->builtins_list)->func(self, error_handler, env_list));
	}
	if (ft_strchr(cmd_content->cmd_name, '/') != NULL)
		exec_ret = execve_in_absolute_path(cmd_content, env_list);
	else
		exec_ret = ft_execvp(cmd_content, env_list);
	// some error occured
	if (exec_ret == -1)
	{
		ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name, strerror(errno));
		ft_printf(STDERR_FILENO, "errno1: %d\n", errno);
		all_clear_exit(self, error_handler, env_list, 1);
	}
	ft_printf(STDERR_FILENO, "errno2: %d\n", errno);
	all_clear_exit(self, error_handler, env_list, get_err_status());
}

bool	parent_process(t_pipes *pipes)
{
	if (pipes->prev_pipe[0] != -1)
	{
		if (close(pipes->prev_pipe[0]) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	}
	if (pipes->prev_pipe[1] != -1)
	{
		if (close(pipes->prev_pipe[1]) == -1)
			return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	}
	return (true);
}

int	execute(t_executor *self, t_error_handler *error_handler, t_list *env_list)
{
	int		status;
	t_cmd	*cmd_content;
	t_list	*head;

	head = self->cmds;
	// print_cmd(self->cmds);
	cmd_content = (t_cmd *)(self->cmds->content);
	if (!self->cmds->next)
	{
		if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
		{
			return (lookup_builtin(cmd_content->cmd_name, \
				self->builtins_list)->func(self, error_handler, env_list));
		}
	}
	while (head)
	{
		self->pipes->prev_pipe[0] = self->pipes->next_pipe[0];
		self->pipes->prev_pipe[1] = self->pipes->next_pipe[1];
		if (head->next)
		{
			if (pipe(self->pipes->next_pipe) == -1)
				return (ft_printf(STDERR_FILENO, "%s: %s\n", "pipe", strerror(errno)), get_err_status());
		}
		cmd_content = (t_cmd *)(head->content);
		cmd_content->pid = fork();
		if (cmd_content->pid == -1)
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "fork", strerror(errno)), get_err_status());
		else if (cmd_content->pid > 0)
		{
			if (!parent_process(self->pipes))
				return (get_err_status());
			head = head->next;
			continue ;
		}
		execute_child_process(self, error_handler, env_list, head);
	}
	waitpid(cmd_content->pid, &status, 0);
	// ft_printf(STDERR_FILENO, "pid:%d status: %d\n", cmd_content->pid, WEXITSTATUS(status));
	// set_shell_varialbe("?", ft_itoa(WEXITSTATUS(status)));
	return (WEXITSTATUS(status));
}

bool	repair_std_io(t_executor *self)
{
	if (close(STDIN_FILENO) == -1)
		return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	if (dup2(self->original_stdin, STDIN_FILENO) == -1)
		return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
	if (close(self->original_stdin) == -1)
		return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	if (close(STDOUT_FILENO) == -1)
		return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
	if (dup2(self->original_stdout, STDOUT_FILENO) == -1)
		return (ft_printf(STDERR_FILENO, "dup2: %s\n", strerror(errno)), false);
	if (close(self->original_stdout) == -1)
		return (ft_printf(STDERR_FILENO, "close: %s\n", strerror(errno)), false);
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
