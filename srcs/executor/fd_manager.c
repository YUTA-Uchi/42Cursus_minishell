/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:19:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/11 18:22:06 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	is_fd_open(int fd)
{
	struct stat	statbuf;

	return (fstat(fd, &statbuf) == 0);
}
