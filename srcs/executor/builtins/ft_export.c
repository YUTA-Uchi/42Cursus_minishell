/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 17:07:08 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_export(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	int		i;
	char	*key;
	char	*value;
	t_cmd	*cmd_content;

	(void)self;
	i = 1;
	cmd_content = (t_cmd *)(current_cmd->content);
	if (!cmd_content->args[i])
	{
		print_env(shell_state->env_list, true);
		return (0);
	}
	while (cmd_content->args[i])
	{
		if (ft_strchr(cmd_content->args[i], '='))
		{
			key = ft_strndup(cmd_content->args[i] \
						, ft_strchr(cmd_content->args[i], '=') \
						- cmd_content->args[i]);
			value = ft_strdup(ft_strchr(cmd_content->args[i], '=') + 1);
			if (!add_env(shell_state->env_list, key, value))
			{
				free(key);
				free(value);
				return (print_strerror("export"), 1);
			}
			free(key);
			free(value);
		}
		else
		{
			if (!get_env_value(shell_state->env_list, cmd_content->args[i]))
				add_env(shell_state->env_list, cmd_content->args[i], NULL);
		}
		i++;
	}
	return (0);
}
