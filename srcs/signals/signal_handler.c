/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:16:03 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/19 14:52:48 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

volatile sig_atomic_t	g_signal;

static void	interactive_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = signum;
		write(STDOUT_FILENO, "\n", 1);
		rl_done = 1;
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

static void	heredoc_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = signum;
		write(STDOUT_FILENO, "\n", 1);
		rl_done = 1;
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}


static void	exec_signal_handler(int signum)
{
	if (signum == SIGINT)
		g_signal = signum;
	else if (signum == SIGQUIT)
		g_signal = signum;
}

int	check_signals(void)
{
	if (g_signal == SIGINT || g_signal == SIGQUIT)
	{
		// rl_done = 1;
		return (0);
	}
	return (0);
}

bool	set_interactive_signal_handler(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = interactive_signal_handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (false);
	signal(SIGQUIT, SIG_IGN);
	g_signal = 0;
	return (true);
}

bool	set_heredoc_signal_handler(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = heredoc_signal_handler;
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		return (false);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		return (false);
	return (true);
}

bool	set_exec_signal_handler(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = exec_signal_handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (false);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (false);
	return (true);
}

bool	set_child_signal_handler(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (true);
}