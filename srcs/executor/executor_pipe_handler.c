/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:52:54 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 14:29:54 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_pipes	*create_pipes(void)
{
	t_pipes	*pipes;

	pipes = malloc(sizeof(t_pipes));
	if (!pipes)
		return (NULL);
	pipes->prev_pipe[0] = -1;
	pipes->prev_pipe[1] = -1;
	pipes->next_pipe[0] = -1;
	pipes->next_pipe[1] = -1;
	return (pipes);
}

bool	set_pipes(t_executor *self, t_list *current_cmd)
{
	if (current_cmd != self->cmds)
	{
		if (!replace_fd(STDIN_FILENO, self->pipes->prev_pipe[0]))
			return (false);
		if (is_fd_open(self->pipes->prev_pipe[1]))
		{
			if (!safe_close(self->pipes->prev_pipe[1]))
				return (false);
		}
	}
	if (current_cmd->next)
	{
		if (is_fd_open(self->pipes->next_pipe[0]))
		{
			if (!safe_close(self->pipes->next_pipe[0]))
				return (false);
		}
		if (!replace_fd(STDOUT_FILENO, self->pipes->next_pipe[1]))
			return (false);
	}
	return (true);
}

void	free_pipes(t_pipes *pipes)
{
	if (is_fd_open(pipes->prev_pipe[0]))
		safe_close(pipes->prev_pipe[0]);
	if (is_fd_open(pipes->prev_pipe[1]))
		safe_close(pipes->prev_pipe[1]);
	if (is_fd_open(pipes->next_pipe[0]))
		safe_close(pipes->next_pipe[0]);
	if (is_fd_open(pipes->next_pipe[1]))
		safe_close(pipes->next_pipe[1]);
	free(pipes);
}
