/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:34:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 14:18:51 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	ft_env(t_cmd *cmd, t_error_handler *error_handler)
{
	int		i;

	i = 0;
	if (cmd->args[1] != NULL)
	{
		set_error(error_handler, E_GENERAL_ERR, ENV_TOO_MANY_ARGS);
		return (1);
	}
	if (__environ == NULL)
	{
		set_error(error_handler, E_GENERAL_ERR, ENV_NULL);
		return (1);
	}
	while (__environ[i])
	{
		ft_printf(STDOUT_FILENO, "%s\n", __environ[i]);
		i++;
	}
	return (0);
}
