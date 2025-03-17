/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:34:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/17 13:14:42 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_env(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	t_cmd	*cmd_content;

	(void)self;
	cmd_content = (t_cmd *)(current_cmd->content);
	if (cmd_content->args[1])
		return (print_const_error(ENV_TOO_MANY_ARGS, E_GENERAL_ERR));
	if (!shell_state->env_list)
		return (print_const_error(ENV_NULL, E_GENERAL_ERR));
	print_env(shell_state->env_list, false);
	return (0);
}
