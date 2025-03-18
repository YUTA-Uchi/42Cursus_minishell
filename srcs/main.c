/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 15:28:54 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_handler.h"
#include "parser.h"
#include "executor.h"
#include "environment.h"
#include "shell_state.h"
#include "signals.h"

extern volatile sig_atomic_t	g_signal;

bool	preprocess(t_shell_state **sh_state, char **environ)
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

bool	set_non_interactive(t_shell_state *sh_state)
{
	if (!isatty(STDIN_FILENO))
	{
		sh_state->running = false;
		sh_state->last_status = E_NOTFOUND;
		return (false);
	}
	return (true);
}

bool	set_cmd_to_exec(t_executor **executor, t_parser **parser \
						, t_shell_state *sh_state)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		free_parser(*parser);
		free_executor(*executor);
		sh_state->last_status = SIGINT | 128;
		return (true);
	}
	(*executor)->cmds = (*parser)->parse(*parser, sh_state);
	free_parser(*parser);
	if (!(*executor)->cmds)
	{
		free_executor(*executor);
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv, char **environ)
{
	t_parser		*parser;
	t_executor		*executor;
	t_shell_state	*sh_state;

	(void)argc;
	(void)argv;
	if (!preprocess(&sh_state, environ))
		return (fatal_error("main", "malloc failed", errno), E_GENERAL_ERR);
	while (sh_state->running)
	{
		executor = create_executor();
		if (!executor)
			continue ;
		parser = create_parser(sh_state);
		if (!parser)
			continue ;
		if (!set_cmd_to_exec(&executor, &parser, sh_state))
			continue ;
		sh_state->last_status = executor->execute(executor, sh_state);
		repair_std_io(executor);
		set_non_interactive(sh_state);
		free_executor(executor);
	}
	all_clear_exit(executor, sh_state, sh_state->last_status);
	return (0);
}
