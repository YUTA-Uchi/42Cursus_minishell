/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:51:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/04/02 18:48:31 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

int	check_signals(void)
{
	if (g_signal == SIGINT || g_signal == SIGQUIT)
	{
		rl_done = 1;
		// write(STDERR_FILENO, "\n", 1);
		// rl_replace_line("", 0);
		// rl_on_new_line();
	}
	return (0);
}

bool	handle_pending_signals(t_shell_state *sh_state)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		sh_state->last_status = SIGINT | E_SIGTERM;
		return (true);
	}
	return (false);
}
