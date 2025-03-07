/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 15:20:03 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_handler.h"
#include "parser.h"
#include "executor.h"
#include "environment.h"
#include "shell_state.h"

int	main(int argc, char **argv, char **environ)
{
	t_error_handler	*error_handler;
	t_parser		*parser;
	t_executor		*executor;
	t_list			*env_list;
	t_shell_state	*shell_state;

	(void)argc;
	(void)argv;
	error_handler = create_error_handler();
	if (!error_handler)
		ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
	env_list = create_env_list(environ);
	if (!env_list)
		ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
	shell_state = create_shell_state();
	if (!shell_state)
		ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
	while (shell_state->running)
	{
		parser = create_parser(error_handler);
		if (!parser)
		{
			ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
			continue ;
		}
		executor = create_executor();
		if (!executor)
		{
			free_parser(parser);
			ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
			continue ;
		}
		executor->cmds = parser->parse(parser, error_handler, env_list, shell_state);
		// TODO here_doc(executor->cmds, error_handler);
		free_parser(parser);
		if (!executor->cmds)
		{
			free_executor(executor);
			continue ;
		}
		shell_state->last_status = executor->execute(executor, error_handler, env_list);
		//ft_printf(STDERR_FILENO, "last_status: %d\n", shell_state->last_status);
		repair_std_io(executor);
		if (!isatty(STDIN_FILENO))
		{
			shell_state->running = false;
			shell_state->last_status = E_NOTFOUND;
			break ;
		}
		free_executor(executor);
		// ft_printf(STDOUT_FILENO, "running_status: %d\n", running_status);
	}
	all_clear_exit(executor, env_list, error_handler, shell_state->last_status);
	return (0);
}
