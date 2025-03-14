/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:16:03 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/14 20:32:54 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

volatile sig_atomic_t	g_signal;

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = signum;
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
	}
}

int	check_signals(void)
{
	if (g_signal == 0)
		return (0);
	else if (g_signal == SIGINT)
	{
		rl_done = 1;
		g_signal = 0;
		return (0);
	}
	return (0);
}

bool	set_signal_handler(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction");
		return (false);
	}
	signal(SIGQUIT, SIG_IGN);
	return (true);
}
