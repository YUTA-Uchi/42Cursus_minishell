/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:41 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/27 17:50:28 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	ft_isdigit_str(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

/*
* need to think about the error handling
*/
int	ft_exit(t_executor *self, t_error_handler *error_handler, t_list *env_list)
{
	int		exit_status;
	t_cmd	*cmd;

	(void)env_list;
	cmd = (t_cmd *)(self->cmds->content);
	// TODO is this correct? 
	if (cmd->args[1] == NULL)
		exit_status = 0;
	else if (ft_isdigit_str(cmd->args[1]))
		exit_status = ft_atoi(cmd->args[1]); // TODO it should be the num 0~255
	else
	{
		set_error(error_handler, E_GENERAL_ERR, "numeric argument required");
		return (1);
	}
	//free_cmd(cmd); // TODO exit should free all the memory
	exit(exit_status);
}
