/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_syscall_wrapper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:13:35 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 11:50:20 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall_wrapper.h"

// pid_t	safe_fork(void)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1)
// 		print_strerror("fork");
// 	return (pid);
// }

// bool	safe_waitpid(pid_t pid, int *status, int options)
// {
// 	pid_t	result;

// 	result = waitpid(pid, status, options);
// 	if (result == -1)
// 	{
// 		if (errno != EINTR)
// 			return (print_strerror("waitpid"), false);
// 	}
// 	return (true);
// }

// void	*safe_malloc_critical(size_t size, const char *context)
// {
// 	void	*ptr;

// 	ptr = malloc(size);
// 	if (!ptr)
// 	{
// 		ft_printf(STDERR_FILENO 
// 				, "Fatal error: memory allocation failed for %s\n" 
// 				, context);
// 		exit(EXIT_FAILURE);
// 	}
// 	return (ptr);
// }

bool	replace_fd(int target_fd, int source_fd)
{
	if (is_fd_open(source_fd))
	{
		if (!safe_close(target_fd))
			return (false);
		if (!safe_dup2(source_fd, target_fd))
			return (false);
		if (!safe_close(source_fd))
			return (false);
	}
	return (true);
}

bool	restore_std_fd(int *saved_stdin, int *saved_stdout)
{
	if (!safe_dup2(*saved_stdin, STDIN_FILENO))
		return (false);
	if (!safe_close(*saved_stdin))
		return (false);
	*saved_stdin = -1;
	if (!safe_dup2(*saved_stdout, STDOUT_FILENO))
		return (false);
	if (!safe_close(*saved_stdout))
		return (false);
	*saved_stdout = -1;
	return (true);
}
