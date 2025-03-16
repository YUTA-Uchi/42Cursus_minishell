/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/14 15:01:29 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_export(t_executor *self, t_shell_state *shell_state)
{
	int		i;
	char	*key;
	char	*value;
	t_cmd	*cmd;

	i = 1;
	cmd = (t_cmd *)(self->cmds->content);
	// if (!cmd->args[i])
	// 	return (print_env(shell_state->env_list), 0); // todo
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '='))
		{
			key = ft_strndup(cmd->args[i] \
							, ft_strchr(cmd->args[i], '=') - cmd->args[i]);
			value = ft_strdup(ft_strchr(cmd->args[i], '=') + 1);
			if (!add_env(shell_state->env_list, key, value))
			{
				free(key);
				free(value);
				return (print_strerror("export"), 1);
			}
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
}
