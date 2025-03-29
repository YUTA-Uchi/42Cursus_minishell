/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/29 20:17:29 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_unset(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	int		i;
	t_cmd	*cmd_content;

	(void)self;
	i = 1;
	cmd_content = (t_cmd *)(current_cmd->content);
	while (cmd_content->args[i])
	{
		remove_env(&shell_state->env_list, cmd_content->args[i]);
		i++;
	}
	return (0);
}
