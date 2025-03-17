/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:16 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/17 14:36:27 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_cd(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	char	*path;
	t_cmd	*cmd_content;

	(void)self;
	cmd_content = (t_cmd *)(current_cmd->content);
	if (!cmd_content->args[1])
	{
		path = get_env_value(shell_state->env_list, "HOME");
		if (!path)
			return (print_const_error(CD_HOME_NOT_SET, E_GENERAL_ERR));
	}
	else
	{
		if (cmd_content->args[2])
			return (print_const_error(CD_TOO_MANY_ARGS, E_GENERAL_ERR));
		path = cmd_content->args[1];
	}
	if (chdir(path) == -1)
		return (print_strerror(path), E_GENERAL_ERR);
	return (0);
}
