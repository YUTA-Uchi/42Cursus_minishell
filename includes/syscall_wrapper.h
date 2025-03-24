/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrapper.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:05:32 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 19:06:17 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_WRAPPER_H
# define SYSCALL_WRAPPER_H

# include "minishell.h"
# include "error_handler.h"

/* fd_core.c */
bool	is_fd_open(int fd);
bool	safe_close(int fd);
bool	safe_dup2(int oldfd, int newfd);
bool	safe_pipe(int pipefd[2]);
int		safe_open(const char *path, int flags, mode_t mode);
/* proc_syscall_wrapper.c */
pid_t	safe_fork(void);
bool	safe_waitpid(pid_t pid, int *status, int options);
void	*safe_malloc_critical(size_t size, const char *context);
bool	replace_fd(int target_fd, int source_fd);
bool	restore_std_fd(int *saved_stdin, int *saved_stdout);

#endif