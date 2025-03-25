/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:26:34 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 14:26:56 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"

int	get_err_status(void)
{
	if (errno == EACCES)
		return (E_NOPERMISSION);
	return (E_NOTFOUND);
}
