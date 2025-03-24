/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:57:03 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 19:55:14 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_state.h"

t_shell_state	*create_shell_state(char **environ)
{
	t_shell_state	*shell_state;

	shell_state = safe_malloc_critical(sizeof(t_shell_state) \
									, "create_shell_state");
	if (!shell_state)
		return (NULL);
	shell_state->running = true;
	shell_state->last_status = 0;
	shell_state->error_handler = create_error_handler();
	if (!shell_state->error_handler)
		return (free_shell_state(shell_state), NULL);
	shell_state->env_list = create_env_list(environ);
	if (!shell_state->env_list)
		return (free_shell_state(shell_state), NULL);
	return (shell_state);
}

void	free_shell_state(t_shell_state *shell_state)
{
	if (shell_state->error_handler)
		free_error_handler(shell_state->error_handler);
	if (shell_state->env_list)
		free_env_list(shell_state->env_list);
	free(shell_state);
}
