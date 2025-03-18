/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:13:35 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 18:47:30 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	xdup2(int fd, t_executor *executor)
{
	int	ret;

	ret = dup(fd);
	if (ret == -1)
	{
		ft_printf(STDERR_FILENO, "dup: %s\n", strerror(errno));
		executor->last_status = E_GENERAL_ERR;
	}
	return (ret);
}
