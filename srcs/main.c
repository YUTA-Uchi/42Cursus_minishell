/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/21 14:43:43 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_handler.h"
#include "parser.h"
#include "executor.h"

int	main(void)
{
	int				running_status;
	t_error_handler	*error_handler;
	t_parser		*parser;
	t_executor		*executor;

	error_handler = create_error_handler();
	if (!error_handler)
		fatal_error("main", "malloc failed");
	running_status = 0;
	while (!running_status)
	{
		parser = create_parser(error_handler);
		if (!parser)
			fatal_error("main", "malloc failed");
		executor = create_executor();
		if (!executor)
			fatal_error("main", "malloc failed");
		executor->cmds = parser->parse(parser, error_handler);
		free_parser(parser);
		running_status = executor->execute(executor, error_handler);
		// ft_printf(STDOUT_FILENO, "running_status: %d\n", running_status);
		repair_std_io(executor);
		free_executor(executor);
		ft_printf(STDOUT_FILENO, "running_status: %d\n", running_status);
	}
	free_error_handler(error_handler);
	exit(0);
}
