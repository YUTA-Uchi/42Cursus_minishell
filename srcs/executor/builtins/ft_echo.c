/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:31:24 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/27 17:50:20 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	ft_echo(t_executor *self, t_error_handler *error_handler, t_list *env_list)
{
	int		i;
	bool	is_option_n;
	t_cmd	*cmd;

	i = 1;
	cmd = (t_cmd *)(self->cmds->content);
	(void)error_handler;
	(void)env_list;
	is_option_n = false;
	if (cmd->args[i] != NULL && ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		is_option_n = true;
		i++;
	}
	while (cmd->args[i] != NULL)
	{
		ft_printf(STDOUT_FILENO, "%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			ft_printf(STDOUT_FILENO, " ");
		i++;
	}
	if (!is_option_n)
		ft_printf(STDOUT_FILENO, "\n");
	return (0);
}
