/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:53:14 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 14:51:49 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include "shell_state.h"

extern volatile sig_atomic_t	g_signal;

int		check_signals(void);

/*
** シグナルハンドラ設定関数
*/
bool	set_interactive_signal_handler(void);
bool	set_heredoc_signal_handler(void);
bool	set_exec_signal_handler(void);
bool	set_child_signal_handler(void);
// main
bool	handle_pending_signals(t_shell_state *sh_state);

#endif