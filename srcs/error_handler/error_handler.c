/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:30:12 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 16:32:13 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_handler.h"

void	fatal_error(const char *function_name, const char *msg)
{
	ft_printf(STDERR_FILENO, "%s%sFatal Error: %s\n", ERR_HEADER, \
			function_name, msg);
	exit(1);
}

int	get_err_status(void)
{
	if (errno == EACCES)
		return (E_NOPERMISSION);
	return (E_NOTFOUND);
}