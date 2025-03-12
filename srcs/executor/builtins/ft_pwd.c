/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:41:49 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_pwd(t_executor *self, t_shell_state *shell_state)
{
	char	*absolute_path_buff;
	t_cmd	*cmd_content;

	(void)shell_state->env_list;
	cmd_content = (t_cmd *)(self->cmds->content);
	if (cmd_content->args[1] != NULL)
	{
		set_error(shell_state->error_handler, E_GENERAL_ERR, PWD_TOO_MANY_ARGS);
		return (1);
	}
	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (getcwd(absolute_path_buff, PATH_MAX) == NULL)
	{
		set_error(shell_state->error_handler, E_GENERAL_ERR, strerror(errno));
		return (1);
	}
	if (!absolute_path_buff)
	{
		set_error(shell_state->error_handler, E_GENERAL_ERR, BUFF_NULL);
		return (1);
	}
	ft_printf(STDOUT_FILENO, "%s\n", absolute_path_buff);
	free(absolute_path_buff);
	return (0);
}
