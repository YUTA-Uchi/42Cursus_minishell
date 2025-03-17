/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:30:12 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/17 12:25:07 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"

void	fatal_error(const char *function_name, const char *msg, int status)
{
	ft_printf(STDERR_FILENO, "%s%sFatal Error: %s\n", ERR_HEADER, \
			function_name, msg);
	exit(status);
}

int	get_err_status(void)
{
	if (errno == EACCES)
		return (E_NOPERMISSION);
	return (E_NOTFOUND);
}

void	set_error(t_error_handler *self, int error, const char *msg)
{
	self->error = error;
	self->msg = msg;
}

int	print_const_error(const char *msg, int status)
{
	ft_printf(STDERR_FILENO, "%s%s\n", ERR_HEADER, msg);
	return (status);
}

void	print_strerror(char *funcname)
{
	ft_printf(STDERR_FILENO, "%s%s: %s\n", ERR_HEADER, \
			funcname, strerror(errno));
}

t_error_handler	*create_error_handler(void)
{
	t_error_handler	*error_handler;

	error_handler = malloc(sizeof(t_error_handler));
	if (!error_handler)
		return (NULL);
	error_handler->error = E_SUCCESS;
	error_handler->msg = NULL;
	return (error_handler);
}

void	free_error_handler(t_error_handler *error_handler)
{
	free(error_handler);
}
