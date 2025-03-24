/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_syscall_wrapper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:19:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 19:07:16 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall_wrapper.h"

bool	is_fd_open(int fd)
{
	struct stat	statbuf;

	return (fstat(fd, &statbuf) == 0);
}

bool	safe_close(int fd)
{
	if (close(fd) == -1)
		return (print_strerror("close"), false);
	return (true);
}

bool	safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		return (print_strerror("dup2"), false);
	return (true);
}

bool	safe_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
		return (print_strerror("pipe"), false);
	return (true);
}

int	safe_open(const char *path, int flags, mode_t mode)
{
	int	fd;

	fd = open(path, flags, mode);
	if (fd == -1)
		print_strerror(path);
	return (fd);
}
