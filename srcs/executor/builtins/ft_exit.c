/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:41 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/29 20:16:23 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	ft_isdigit_str(char *str)
{
	int		i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	int		exit_status;
	t_cmd	*cmd;

	cmd = (t_cmd *)(current_cmd->content);
	if (shell_state->is_interactive)
		ft_printf(STDERR_FILENO, "exit\n");
	if (!cmd->args[1])
		terminate_shell(self, shell_state, shell_state->last_status);
	if (ft_isdigit_str(cmd->args[1]))
		exit_status = ft_atoi(cmd->args[1]) % 256;
	else
	{
		print_error_with_status(EXIT_NUMERIC_ARG, E_GENERAL_ERR);
		exit_status = 2;
		terminate_shell(self, shell_state, exit_status);
	}
	if (cmd->args[2])
		return (print_error_with_status(EXIT_TOO_MANY_ARGS, E_GENERAL_ERR));
	terminate_shell(self, shell_state, exit_status);
	return (0);
}
