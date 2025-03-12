/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:34:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:40:22 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_env(t_executor *self, t_shell_state *shell_state)
{
	t_list	*current;
	t_cmd	*cmd;

	current = shell_state->env_list;
	cmd = (t_cmd *)(self->cmds->content);
	if (cmd->args[1] != NULL)
	{
		set_error(shell_state->error_handler, E_GENERAL_ERR, ENV_TOO_MANY_ARGS);
		return (1);
	}
	if (current == NULL)
	{
		set_error(shell_state->error_handler, E_GENERAL_ERR, ENV_NULL);
		return (1);
	}
	while (current)
	{
		ft_printf(STDOUT_FILENO, "%s=%s\n" \
				, ((t_env *)(current->content))->key \
				, ((t_env *)(current->content))->value);
		current = current->next;
	}
	return (0);
}
