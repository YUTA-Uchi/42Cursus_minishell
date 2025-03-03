/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:16 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/27 17:50:15 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_cd(t_executor *self, t_error_handler *error_handler, t_list *env_list)
{
	char	*path;
	t_cmd	*cmd;

	cmd = (t_cmd *)(self->cmds->content);
	if (cmd->args[1] == NULL)
	{
		path = get_env_value(env_list, "HOME");
		if (path == NULL)
			return (set_error(error_handler, E_GENERAL_ERR, CD_HOME_NOT_SET) \
					, 1);
	}
	else
	{
		if (cmd->args[2] != NULL)
			return (set_error(error_handler, E_GENERAL_ERR, CD_TOO_MANY_ARGS) \
					, 1);
		path = cmd->args[1];
	}
	if (chdir(path) == -1)
		return (set_error(error_handler, E_GENERAL_ERR, strerror(errno)) \
				, 1);
	return (0);
}
