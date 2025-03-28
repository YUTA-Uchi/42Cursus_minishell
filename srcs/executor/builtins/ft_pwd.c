/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 19:47:44 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	print_env_pwd(t_shell_state *shell_state)
{
	if (get_env_value(shell_state->env_list, "PWD"))
	{
		ft_printf(STDOUT_FILENO, "%s\n" \
				, get_env_value(shell_state->env_list, "PWD"));
		return (true);
	}
	return (false);
}

bool	print_pwd_from_getcwd(void)
{
	char	*absolute_path_buff;

	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (!absolute_path_buff)
		return (print_error_with_status(BUFF_NULL, E_GENERAL_ERR));
	if (!getcwd(absolute_path_buff, PATH_MAX))
	{
		free(absolute_path_buff);
		return (print_strerror("getcwd"), false);
	}
	ft_printf(STDOUT_FILENO, "%s\n", absolute_path_buff);
	free(absolute_path_buff);
	return (true);
}

int	ft_pwd(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	// t_cmd	*cmd_content;

	(void)self;
	(void)current_cmd;
	// cmd_content = (t_cmd *)(current_cmd->content);
	// if (cmd_content->args[1])
	// 	return (print_error_with_status(PWD_TOO_MANY_ARGS, E_GENERAL_ERR));
	if (print_env_pwd(shell_state))
		return (0);
	if (!print_pwd_from_getcwd())
		return (0);
	return (E_GENERAL_ERR);
}
