/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:31:24 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 14:14:22 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	ft_echo(t_cmd *self, t_error_handler *error_handler)
{
	int		i;
	bool	is_option_n;

	i = 1;
	(void)error_handler;
	is_option_n = false;
	if (self->args[i] != NULL && ft_strncmp(self->args[i], "-n", 2) == 0)
	{
		is_option_n = true;
		i++;
	}
	while (self->args[i] != NULL)
	{
		ft_printf(STDOUT_FILENO, "%s", self->args[i]);
		if (self->args[i + 1] != NULL)
			ft_printf(STDOUT_FILENO, " ");
		i++;
	}
	if (!is_option_n)
		ft_printf(STDOUT_FILENO, "\n");
	return (0);
}
