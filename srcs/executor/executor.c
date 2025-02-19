/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 16:59:02 by yuuchiya         ###   ########.fr       */
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

int	excute(t_executor *self)
{
	int	status;
	int	exec_ret;

	if (lookup_builtin(self->cmds->cmd_name, self->builtins_list)->name)
	{
		exec_ret = lookup_builtin(self->cmds->cmd_name, \
				self->builtins_list)->func(self->cmds, self->error_handler);
		return (exec_ret);
	}
	self->cmds->pid = fork();
	if (self->cmds->pid == -1)
		fatal_error("fork: ", strerror(errno));
	else if (self->cmds->pid == 0)
	{
		if (ft_strchr(self->cmds->cmd_name, '/') != NULL)
			exec_ret = execve_in_absolute_path(self->cmds);
		else
			exec_ret = ft_execvp(self->cmds);
		if (exec_ret != 0)
			return (exec_ret);
		fatal_error("", COMMAND_NOT_FOUND);
	}
	else
	{
		waitpid(self->cmds->pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (0);
}

t_executor	*create_executor(void)
{
	t_executor			*executor;

	executor = malloc(sizeof(t_executor));
	if (!executor)
		return (NULL);
	executor->cmds = NULL;
	executor->excute = excute;
	executor->error_handler = create_error_handler();
	executor->builtins_list = create_builtins_list();
	return (executor);
}

void	free_executor(t_executor *executor)
{
	if (executor->cmds)
		free_cmd(executor->cmds);
	free(executor);
}
