/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 15:43:15 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	ft_unset(t_cmd *cmd, t_error_handler *error_handler)
{
	int		i;

	i = 1;
	while (cmd->args[i])
	{
		if (unsetenv(cmd->args[i]) == -1) // unsetenv could not be used
			return (set_error(error_handler, E_GENERAL_ERR, strerror(errno)), 1);
		i++;
	}
	return (0);
}