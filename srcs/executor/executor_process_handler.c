/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:55:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/11 18:25:19 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"

static char	*create_path(char *env_path, char *cmd)
{
	char	*path_part;
	char	*exec_path;

	path_part = ft_strjoin(env_path, "/");
	exec_path = ft_strjoin(path_part, cmd);
	free(path_part);
	return (exec_path);
}

static int	execve_in_absolute_path(t_cmd *cmd, t_list *env_list)
{
	if (access(cmd->cmd_name, F_OK) == 0)
		return (execve(cmd->cmd_name, cmd->args \
				, (char *const *)env_list_to_array(env_list)));
	return (ft_printf(STDERR_FILENO, "executor: %s\n" \
			, strerror(errno)), get_err_status());
}

static int	ft_execvp(t_cmd *cmd, t_list *env_list)
{
	int				i;
	char			**env_pathes;
	char			*exec_path;

	env_pathes = ft_split(get_env_value(env_list, "PATH"), ':');
	if (env_pathes == NULL)
		return (ft_printf(STDERR_FILENO, "executor: %s\n" \
				, "malloc failed"), get_err_status());
	i = 0;
	while (env_pathes[i])
	{
		exec_path = create_path(env_pathes[i], cmd->cmd_name);
		if (access(exec_path, F_OK) == 0)
		{
			free_arr(env_pathes);
			return (execve(exec_path, cmd->args \
					, (char *const *)env_list_to_array(env_list)));
		}
		free(exec_path);
		i++;
	}
	free_arr(env_pathes);
	return (get_err_status());
}

void	execute_child_process(t_executor *self, t_list *env_list \
					, t_list *current_cmd, t_error_handler *error_handler)
{
	t_cmd	*cmd_content;
	int		exec_ret;

	if (!set_pipes(self, current_cmd, error_handler))
		all_clear_exit(self, env_list, error_handler, errno);
	if (!set_redirections(current_cmd))
		all_clear_exit(self, env_list, error_handler, errno);
	cmd_content = (t_cmd *)(current_cmd->content);
	if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
		exit(lookup_builtin(cmd_content->cmd_name, \
			self->builtins_list)->func(self, error_handler, env_list));
	if (ft_strchr(cmd_content->cmd_name, '/') != NULL)
		exec_ret = execve_in_absolute_path(cmd_content, env_list);
	else
		exec_ret = ft_execvp(cmd_content, env_list);
	if (exec_ret == -1)
	{
		if (errno == EACCES)
			ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
					, COMMAND_NOT_FOUND);
		else
			ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
					, strerror(errno));
		all_clear_exit(self, env_list, error_handler, get_err_status());
	}
	ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
			, COMMAND_NOT_FOUND);
	all_clear_exit(self, env_list, error_handler, get_err_status());
}

bool	parent_process(t_pipes *pipes)
{
	if (pipes->prev_pipe[0] > -1 && is_fd_open(pipes->prev_pipe[0]))
	{
		if (close(pipes->prev_pipe[0]) == -1)
			return (print_strerror("close"), false);
	}
	if (pipes->prev_pipe[1] > -1 && is_fd_open(pipes->prev_pipe[1]))
	{
		if (close(pipes->prev_pipe[1]) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}
