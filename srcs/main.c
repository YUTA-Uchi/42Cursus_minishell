/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/17 18:30:12 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fatal_error(const char *function_name, const char *msg) __attribute__((noreturn));

void	fatal_error(const char *function_name, const char *msg)
{
	dprintf(STDERR_FILENO, "%s%sFatal Error: %s\n", ERR_HEADER, function_name, msg);
	exit(1);
}

int	get_err_status(void)
{
	if (errno == EACCES)
		return (E_NOPERMISSION);
	return (E_NOTFOUND);
}

void	free_arr(char **data)
{
	char	**data_origin;

	data_origin = data;
	while (*data)
	{
		free(*data);
		data++;
	}
	free(data_origin);
}

t_cmd	*create_cmd(char *line)
{
	t_cmd		*cmd;
	char		*args[] = {line, NULL};

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit(1);
	cmd->cmd = strdup(line);
	if (!cmd->cmd)
		exit(1);
	cmd->args = args;
	cmd->pid = 0;
	return (cmd);
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

int	execve_in_absolute_path(t_cmd *cmd)
{
	if (access(cmd->cmd, F_OK) == 0)
		return (execve(cmd->cmd, cmd->args, NULL));
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
		exec_path = create_path(env_pathes[i], cmd->cmd);
		if (access(exec_path, F_OK) == 0)
		{
			free_arr(env_pathes);
			return (execve(exec_path, cmd->args, NULL));
		}
		free(exec_path);
		i++;
	}
	free_arr(env_pathes);
	return (fatal_error("", strerror(errno)), get_err_status());
}

int	excute(t_cmd *cmd)
{
	int	status;
	int	exec_ret;

	cmd->pid = fork();
	if (cmd->pid == -1)
		fatal_error("fork: ", strerror(errno));
	else if (cmd->pid == 0)
	{
		if (ft_strchr(cmd->cmd, '/') != NULL)
			exec_ret = execve_in_absolute_path(cmd);
		else
			exec_ret = ft_execvp(cmd);
		if (exec_ret != 0)
			return (exec_ret);
		fatal_error("", COMMAND_NOT_FOUND);
	}
	else
	{
		waitpid(cmd->pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (0);
}

int	main(void)
{
	char	*line;
	t_cmd	*cmd;

	rl_outstream = stderr;
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		cmd = create_cmd(line);
		excute(cmd);
		free(line);
	}
	exit(0);
}
