/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:16 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 16:47:04 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	validate_and_set_target_path(char **target_path, t_cmd *cmd_content \
									, t_shell_state *shell_state)
{
	if (!cmd_content->args[1])
	{
		*target_path = get_env_value(shell_state->env_list, "HOME");
		if (!*target_path)
			return (print_error(CD_HOME_NOT_SET));
	}
	else
	{
		if (cmd_content->args[2])
			return (print_error(CD_TOO_MANY_ARGS));
		*target_path = cmd_content->args[1];
	}
	return (true);
}

bool	update_pwd_from_getcwd(t_shell_state *shell_state)
{
	char	*absolute_path_buff;

	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (!absolute_path_buff)
		return (print_error(BUFF_NULL));
	if (!getcwd(absolute_path_buff, PATH_MAX))
	{
		free(absolute_path_buff);
		return (print_strerror("getcwd"), false);
	}
	if (!set_env_value(shell_state->env_list, "PWD", absolute_path_buff))
	{
		free(absolute_path_buff);
		return (print_error(BUFF_NULL));
	}
	free(absolute_path_buff);
	return (true);
}

int	ft_cd(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	char	*target_path;
	t_cmd	*cmd_content;

	(void)self;
	cmd_content = (t_cmd *)(current_cmd->content);
	if (!validate_and_set_target_path(&target_path, cmd_content, shell_state))
		return (E_GENERAL_ERR);
	if (chdir(target_path) == -1)
		return (print_strerror(target_path), E_GENERAL_ERR);
	if (!update_pwd_from_getcwd(shell_state))
		return (E_GENERAL_ERR);
	return (0);
}
