/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:49:25 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_unset(t_executor *self, t_shell_state *shell_state)
{
	int		i;
	t_cmd	*cmd;

	i = 1;
	cmd = (t_cmd *)(self->cmds->content);
	while (cmd->args[i])
	{
		if (remove_env(&shell_state->env_list, cmd->args[i]) == -1)
			return (set_error(shell_state->error_handler, E_GENERAL_ERR, strerror(errno)), 1);
		i++;
	}
	return (0);
}
