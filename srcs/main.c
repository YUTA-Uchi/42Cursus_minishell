/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/02 18:12:53 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_handler.h"
#include "parser.h"
#include "executor.h"
#include "environment.h"

int	main(int argc, char **argv, char **environ)
{
	int				running_status;
	t_error_handler	*error_handler;
	t_parser		*parser;
	t_executor		*executor;
	t_list			*env_list;

	(void)argc;
	(void)argv;
	error_handler = create_error_handler();
	if (!error_handler)
		ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
	env_list = create_env_list(environ);
	if (!env_list)
		ft_printf(STDERR_FILENO, "%s: %s\n", "main", "malloc failed");
	running_status = 0;
	while (!running_status)
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
		executor->cmds = parser->parse(parser, error_handler, env_list);
		// TODO here_doc(executor->cmds, error_handler);
		free_parser(parser);
		running_status = executor->execute(executor, error_handler, env_list);
		// ft_printf(STDOUT_FILENO, "running_status: %d\n", running_status);
		if (!repair_std_io(executor))
			running_status = get_err_status();
		free_executor(executor);
		// ft_printf(STDOUT_FILENO, "running_status: %d\n", running_status);
	}
	free_error_handler(error_handler);
	exit(0);
}
