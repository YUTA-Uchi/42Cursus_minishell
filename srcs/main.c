/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:29:19 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_handler.h"
#include "parser.h"
#include "executor.h"
#include "environment.h"
#include "shell_state.h"


bool	preprocess(t_shell_state **sh_state, char **environ)
{
	*sh_state = create_shell_state(environ);
	if (!*sh_state)
		return (false);
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
		parser = create_parser(sh_state);
		if (!parser)
		{
			print_strerror("main");
			continue ;
		}
		executor = create_executor();
		if (!executor)
		{
			free_parser(parser);
			print_strerror("main");
			continue ;
		}
		executor->cmds = parser->parse(parser, sh_state);
		free_parser(parser);
		if (!executor->cmds)
		{
			free_executor(executor);
			continue ;
		}
		sh_state->last_status = executor->execute(executor, sh_state);
		repair_std_io(executor);
		if (!isatty(STDIN_FILENO))
		{
			sh_state->running = false;
			sh_state->last_status = E_NOTFOUND;
			break ;
		}
		free_executor(executor);
	}
	all_clear_exit(executor, sh_state, sh_state->last_status);
	return (0);
}
