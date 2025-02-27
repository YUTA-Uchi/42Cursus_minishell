/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/27 17:49:49 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"
#include "builtin.h"

int	ft_unset(t_executor *self, t_error_handler *error_handler, t_list *env_list)
{
	int		i;
	t_cmd	*cmd;

	i = 1;
	cmd = (t_cmd *)(self->cmds->content);
	(void)env_list;
	while (cmd->args[i])
	{
		if (unsetenv(cmd->args[i]) == -1) // unsetenv could not be used
			return (set_error(error_handler, E_GENERAL_ERR, strerror(errno)), 1);
		i++;
	}
	return (0);
}