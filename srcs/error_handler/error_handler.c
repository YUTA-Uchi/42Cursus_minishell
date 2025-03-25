/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:30:12 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 14:33:43 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"

void	fatal_error(const char *function_name, const char *msg, int status)
{
	ft_printf(STDERR_FILENO, "%s%sFatal Error: %s\n", ERR_HEADER, \
			function_name, msg);
	exit(status);
}

bool	print_error(const char *msg)
{
	ft_printf(STDERR_FILENO, "%s%s\n", ERR_HEADER, msg);
	return (false);
}

int	print_error_with_status(const char *msg, int status)
{
	ft_printf(STDERR_FILENO, "%s%s\n", ERR_HEADER, msg);
	return (status);
}

bool	print_error_with_value(const char *err_msg, const char *variable_value)
{
	ft_printf(STDERR_FILENO, err_msg, ERR_HEADER, variable_value);
	return (false);
}

void	print_strerror(const char *funcname)
{
	ft_printf(STDERR_FILENO, "%s%s: %s\n", ERR_HEADER, \
			funcname, strerror(errno));
}
