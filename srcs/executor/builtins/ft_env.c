/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:34:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/22 18:57:46 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"
#include "builtin.h"

int	ft_env(t_cmd *cmd, t_error_handler *error_handler, t_list *env_list)
{
	t_list	*current;

	current = env_list;
	if (cmd->args[1] != NULL)
	{
		set_error(error_handler, E_GENERAL_ERR, ENV_TOO_MANY_ARGS);
		return (1);
	}
	if (current == NULL)
	{
		set_error(error_handler, E_GENERAL_ERR, ENV_NULL);
		return (1);
	}
	while (current)
	{
		ft_printf(STDOUT_FILENO, "%s=%s\n" \
				, ((t_env *)(env_list->content))->key \
				, ((t_env *)(env_list->content))->value);
		current = current->next;
	}
	return (0);
}
