/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:55:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 17:05:51 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"

extern volatile sig_atomic_t	g_signal;

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
	return (-1);
}

static int	ft_execvp(t_cmd *cmd, t_list *env_list)
{
	int				i;
	char			**env_pathes;
	char			*exec_path;

	env_pathes = ft_split(get_env_value(env_list, "PATH"), ':');
	if (env_pathes == NULL)
		return (errno = 2, -1);
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

void	execute_child_process(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state)
{
	t_cmd	*cmd_content;
	int		exec_ret;

	if (!set_pipes(self, current_cmd, shell_state->error_handler))
		terminate_shell(self, shell_state, errno);
	if (!set_redirections(current_cmd))
		terminate_shell(self, shell_state, errno);
	cmd_content = (t_cmd *)(current_cmd->content);
	if (lookup_builtin(cmd_content->cmd_name, self->builtins_list)->name)
		exit(lookup_builtin(cmd_content->cmd_name, \
			self->builtins_list)->func(self, current_cmd, shell_state));
	if (ft_strchr(cmd_content->cmd_name, '/') != NULL)
		exec_ret = execve_in_absolute_path(cmd_content, shell_state->env_list);
	else
		exec_ret = ft_execvp(cmd_content, shell_state->env_list);
	if (exec_ret == -1)
	{
		if (errno == EACCES)
			ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
					, COMMAND_NOT_FOUND);
		else
			ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
					, strerror(errno));
		terminate_shell(self, shell_state, get_err_status());
	}
	ft_printf(STDERR_FILENO, "%s: %s\n", cmd_content->cmd_name \
			, COMMAND_NOT_FOUND);
	terminate_shell(self, shell_state, get_err_status());
}

bool	parent_process(t_pipes *pipes, t_list *redir_list)
{
	t_redirection	*redir_content;

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
	while (redir_list)
	{
		redir_content = (t_redirection *)(redir_list->content);
		if (is_fd_open(redir_content->fd))
		{
			if (close(redir_content->fd) == -1)
				return (print_strerror("close"), false);
		}
		redir_list = redir_list->next;
	}
	return (true);
}
