/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:31:24 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/22 22:03:18 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "signals.h"

bool	is_option_n(char *arg)
{
	if (!arg)
		return (false);
	if (*arg && ft_strncmp(arg, "-n", 3) == 0)
	{
		return (true);
	}
	return (false);
}

int	ft_echo(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	int		i;
	bool	is_option_n_flag;
	t_cmd	*cmd_content;

	i = 1;
	cmd_content = (t_cmd *)(current_cmd->content);
	(void)shell_state;
	(void)self;
	is_option_n_flag = is_option_n(cmd_content->args[i]);
	if (is_option_n_flag)
		i++;
	while (cmd_content->args[i])
	{
		ft_printf(STDOUT_FILENO, "%s", cmd_content->args[i]);
		if (cmd_content->args[i + 1])
			ft_printf(STDOUT_FILENO, " ");
		i++;
	}
	if (!is_option_n_flag)
		ft_printf(STDOUT_FILENO, "\n");
	return (0);
}
