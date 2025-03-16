/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:16 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:39:29 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_cd(t_executor *self, t_shell_state *shell_state)
{
	char	*path;
	t_cmd	*cmd;

	cmd = (t_cmd *)(self->cmds->content);
	if (cmd->args[1] == NULL)
	{
		path = get_env_value(shell_state->env_list, "HOME");
		if (path == NULL)
			return (set_error(shell_state->error_handler, E_GENERAL_ERR, CD_HOME_NOT_SET) \
					, 1);
	}
	else
	{
		if (cmd->args[2] != NULL)
			return (set_error(shell_state->error_handler, E_GENERAL_ERR, CD_TOO_MANY_ARGS) \
					, 1);
		path = cmd->args[1];
	}
	if (chdir(path) == -1)
		return (print_strerror(path) \
				, 1);
	return (0);
}
