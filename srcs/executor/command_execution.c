/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:07:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 15:02:58 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	execve_in_absolute_path(t_cmd *cmd, t_list *env_list)
{
	char	**env_array;
	int		ret;

	env_array = env_list_to_array(env_list);
	if (!env_array)
		return (errno = ENOMEM, -1);
	if (access(cmd->cmd_name, F_OK) == 0)
	{
		ret = execve(cmd->cmd_name, cmd->args, env_array);
		free_arr(env_array);
		return (ret);
	}
	free_arr(env_array);
	return (-1);
}

static int	try_execute_in_path(char *path, t_cmd *cmd, char **env_array)
{
	char	*path_part;
	char	*exec_path;
	int		ret;

	path_part = ft_strjoin(path, "/");
	exec_path = ft_strjoin(path_part, cmd->cmd_name);
	free(path_part);
	if (!exec_path)
		return (ENOMEM);
	if (access(exec_path, F_OK) == 0)
	{
		ret = execve(exec_path, cmd->args, env_array);
		free(exec_path);
		if (ret == -1 && errno == EACCES)
			return (EACCES);
		return (errno);
	}
	free(exec_path);
	return (0);
}

static bool	init_path_execution(t_list *env_list, char ***env_pathes \
								, char ***env_array)
{
	*env_pathes = ft_split(get_env_value(env_list, "PATH"), ':');
	if (!*env_pathes)
	{
		errno = ENOENT;
		return (false);
	}
	*env_array = env_list_to_array(env_list);
	if (!*env_array)
	{
		free_arr(*env_pathes);
		errno = ENOMEM;
		return (false);
	}
	return (true);
}

static int	execute_command_in_paths(t_cmd *cmd, char **env_pathes \
	, char **env_array)
{
	int	i;
	int	ret;

	i = 0;
	while (env_pathes[i])
	{
		ret = try_execute_in_path(env_pathes[i], cmd, env_array);
		if (ret == EACCES)
		{
			free_arr(env_pathes);
			free_arr(env_array);
			return (-1);
		}
		else if (ret == ENOMEM)
		{
			free_arr(env_pathes);
			free_arr(env_array);
			return (-1);
		}
		i++;
	}
	free_arr(env_pathes);
	free_arr(env_array);
	return (get_err_status());
}

int	ft_execvp(t_cmd *cmd, t_list *env_list)
{
	char	**env_pathes;
	char	**env_array;

	if (!init_path_execution(env_list, &env_pathes, &env_array))
		return (-1);
	return (execute_command_in_paths(cmd, env_pathes, env_array));
}
