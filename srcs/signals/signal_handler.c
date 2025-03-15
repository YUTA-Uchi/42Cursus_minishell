/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:16:03 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/15 18:00:48 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

volatile sig_atomic_t	g_signal;

/*
** シグナルハンドラ関数 - インタラクティブモード用
** SIGINT (Ctrl+C) を受け取った場合、g_signalを更新し新しい行に移動
*/
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

/*
** シグナルハンドラ関数 - heredocモード用
** SIGINT (Ctrl+C) を受け取った場合、g_signalを更新し処理を中断
*/
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

/*
** シグナルハンドラ関数 - 子プロセス実行中用
** 親プロセスがシグナルを無視するように設定
*/
static void	exec_signal_handler(int signum)
{
	if (signum == SIGINT)
		g_signal = signum;
	else if (signum == SIGQUIT)
		g_signal = signum;
}

/*
** readline のイベントフックコールバック
** シグナル状態をチェックして必要な処理を行う
*/
int	check_signals(void)
{
	if (g_signal == SIGINT || g_signal == SIGQUIT)
	{
		// rl_done = 1;
		return (0);
	}
	return (0);
}

/*
** インタラクティブモード用のシグナルハンドラを設定
*/
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

/*
** heredocモード用のシグナルハンドラを設定
*/
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

/*
** コマンド実行中用のシグナルハンドラを設定
*/
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

/*
** 子プロセス用のシグナルハンドラを設定（デフォルトの動作に戻す）
*/
bool	set_child_signal_handler(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (true);
}