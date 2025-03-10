/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:52:54 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/10 19:25:21 by yuuchiya         ###   ########.fr       */
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

bool	set_pipes(t_executor *self, t_list *current_cmd \
				, t_error_handler *error_handler)
{
	(void)error_handler;
	if (current_cmd != self->cmds)
	{
		if (close(STDIN_FILENO) == -1)
			return (print_strerror("close"), false);
		if (dup2(self->pipes->prev_pipe[0], STDIN_FILENO) == -1)
			return (print_strerror("dup2"), false);
		if (close(self->pipes->prev_pipe[0]) == -1)
			return (print_strerror("close"), false);
		if (close(self->pipes->prev_pipe[1]) == -1)
			return (print_strerror("close"), false);
	}
	if (current_cmd->next)
	{
		if (close(self->pipes->next_pipe[0]) == -1)
			return (print_strerror("close"), false);
		if (close(STDOUT_FILENO) == -1)
			return (print_strerror("close"), false);
		if (dup2(self->pipes->next_pipe[1], STDOUT_FILENO) == -1)
			return (print_strerror("dup2"), false);
		if (close(self->pipes->next_pipe[1]) == -1)
			return (print_strerror("close"), false);
	}
	return (true);
}

void	free_pipes(t_pipes *pipes)
{
	free(pipes);
}
