/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_core_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 14:56:08 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "command_processor.h"
#include "error_handler.h"
#include "environment.h"
#include "shell_state.h"
#include "signals.h"

bool	initialize_shell(t_shell_state **sh_state, char **environ)
{
	*sh_state = create_shell_state(environ);
	if (!(*sh_state))
		return (false);
	if (!set_interactive_signal_handler())
	{
		free_shell_state(*sh_state);
		return (false);
	}
	rl_event_hook = check_signals;
	return (true);
}

bool	check_terminal_state(t_shell_state *sh_state)
{
	if (!isatty(STDIN_FILENO))
	{
		sh_state->running = false;
		sh_state->last_status = E_NOTFOUND;
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv, char **environ)
{
	t_shell_state	*sh_state;

	(void)argc;
	(void)argv;
	if (!initialize_shell(&sh_state, environ))
		return (fatal_error("main", "initialization failed", errno) \
				, E_GENERAL_ERR);
	while (sh_state->running)
	{
		if (!process_command_line(sh_state))
			continue ;
		if (!check_terminal_state(sh_state))
			break ;
	}
	return (terminate_shell(NULL, sh_state, sh_state->last_status));
}
