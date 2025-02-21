/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/21 12:53:11 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

static char	*create_path(char *env_path, char *cmd)
{
	char	*path_part;
	char	*exec_path;

	path_part = ft_strjoin(env_path, "/");
	exec_path = ft_strjoin(path_part, cmd);
	free(path_part);
	return (exec_path);
}

int	execve_in_absolute_path(t_cmd *cmd)
{
	if (access(cmd->cmd_name, F_OK) == 0)
		return (execve(cmd->cmd_name, cmd->args, __environ));
	return (fatal_error("", strerror(errno)), get_err_status());
}

int	ft_execvp(t_cmd *cmd)
{
	int				i;
	char			**env_pathes;
	char			*exec_path;

	env_pathes = ft_split(getenv("PATH"), ':');
	if (env_pathes == NULL)
		return (fatal_error("ft_split: ", strerror(errno)), get_err_status());
	i = 0;
	while (env_pathes[i])
	{
		exec_path = create_path(env_pathes[i], cmd->cmd_name);
		if (access(exec_path, F_OK) == 0)
		{
			free_arr(env_pathes);
			return (execve(exec_path, cmd->args, __environ));
		}
		free(exec_path);
		i++;
	}
	free_arr(env_pathes);
	return (fatal_error("", strerror(errno)), get_err_status());
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

void	execute_child_process(t_executor *self, t_error_handler *error_handler)
{
	t_cmd	*cmd_content;
	int		exec_ret;

	cmd_content = (t_cmd *)(self->cmds->content);
	if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
	{
		exit(lookup_builtin(cmd_content->cmd_name, \
			self->builtins_list)->func(cmd_content, error_handler));
	}
	if (ft_strchr(cmd_content->cmd_name, '/') != NULL)
		exec_ret = execve_in_absolute_path(cmd_content);
	else
		exec_ret = ft_execvp(cmd_content);
	ft_printf(STDOUT_FILENO, "minishell: %i\n", exec_ret);
	if (exec_ret != 0)
		exit(exec_ret);
	fatal_error("", COMMAND_NOT_FOUND);
	exit(1);
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

void	parent_process(t_pipes *pipes)
{
	if (pipes->prev_pipe[0] != -1)
		close(pipes->prev_pipe[0]);
	if (pipes->prev_pipe[1] != -1)
		close(pipes->prev_pipe[1]);
}

int	execute(t_executor *self, t_error_handler *error_handler)
{
	int		status;
	t_cmd	*cmd_content;
	t_pipes	*pipes;
	t_list	*head;

	head = self->cmds;
	// print_cmd(self->cmds);
	cmd_content = (t_cmd *)(self->cmds->content);
	while (!self->cmds->next)
	{
		if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
		{
			return (lookup_builtin(cmd_content->cmd_name, \
				self->builtins_list)->func((t_cmd *)(self->cmds->content), error_handler));
		}
	}
	pipes = create_pipes();
	while (self->cmds)
	{
		pipes->prev_pipe[0] = pipes->next_pipe[0];
		pipes->prev_pipe[1] = pipes->next_pipe[1];
		if (self->cmds->next)
		{
			if (pipe(pipes->next_pipe) == -1)
				return (fatal_error("pipe", strerror(errno)), get_err_status());
		}
		cmd_content = (t_cmd *)(self->cmds->content);
		cmd_content->pid = fork();
		if (cmd_content->pid == -1)
			return (fatal_error("", strerror(errno)), get_err_status());
		else if (cmd_content->pid > 0)
		{
			parent_process(pipes);
			self->cmds = self->cmds->next;
			continue ;
		}
		if (self->cmds != head)
		{
			close(STDIN_FILENO);
			dup2(pipes->prev_pipe[0], STDIN_FILENO);
			close(pipes->prev_pipe[0]);
			close(pipes->prev_pipe[1]);
		}
		if (self->cmds->next)
		{
			close(pipes->next_pipe[0]);
			close(STDOUT_FILENO);
			dup2(pipes->next_pipe[1], STDOUT_FILENO);
			close(pipes->next_pipe[1]);
		}
		execute_child_process(self, error_handler);
	}
	waitpid(cmd_content->pid, &status, 0);
	// ft_printf(STDERR_FILENO, "pid:%d status: %d\n", cmd_content->pid, WEXITSTATUS(status));
	return (WEXITSTATUS(status));
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
	return (executor);
}

void	free_executor(t_executor *executor)
{
	if (executor->cmds)
		free_cmd(executor->cmds);
	free(executor);
}
