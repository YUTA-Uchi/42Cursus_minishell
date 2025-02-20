/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:27:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 19:36:49 by yuuchiya         ###   ########.fr       */
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

int	excute(t_executor *self, t_error_handler *error_handler)
{
	int		status;
	int		exec_ret;
	t_cmd	*cmd_content;

	cmd_content = (t_cmd *)(self->cmds->content);
	int	i = 0;
	while (cmd_content->args[i])
	{
		ft_printf(STDOUT_FILENO, "args[%d]: %s\n", i, cmd_content->args[i]);
		i++;
	}
	if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
	{
		exec_ret = lookup_builtin(cmd_content->cmd_name, \
				self->builtins_list)->func((t_cmd *)(self->cmds->content), error_handler);
		return (exec_ret);
	}
	cmd_content->pid = fork();
	if (cmd_content->pid == -1)
		fatal_error("fork: ", strerror(errno));
	else if (cmd_content->pid == 0)
	{
		if (ft_strchr(cmd_content->cmd_name, '/') != NULL)
			exec_ret = execve_in_absolute_path((t_cmd *)(self->cmds->content));
		else
			exec_ret = ft_execvp((t_cmd *)(self->cmds->content));
		ft_printf(STDOUT_FILENO, "minishell: %i\n", exec_ret);
		if (exec_ret != 0)
			return (exec_ret);
		fatal_error("", COMMAND_NOT_FOUND);
	}
	else
	{
		waitpid(cmd_content->pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}

t_executor	*create_executor(void)
{
	t_executor			*executor;

	executor = malloc(sizeof(t_executor));
	if (!executor)
		return (NULL);
	executor->cmds = NULL;
	executor->excute = excute;
	executor->builtins_list = create_builtins_list();
	return (executor);
}

void	free_executor(t_executor *executor)
{
	if (executor->cmds)
		free_cmd(executor->cmds);
	free(executor);
}
