/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 19:16:46 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_manager.h"

int	main(void)
{
	t_shell_manager	*manager;
	int				running_status;

	manager = create_shell_manager();
	if (!manager)
		exit(1);
	running_status = 0;
	while (!running_status)
	{
		set_line(manager);
		if (!manager->parser->line)
			continue ;
		set_cmds(manager);
		// if (!manager->executor->cmds)
		// 	// TODO error handling parser error;
		running_status = manager->executor->excute(manager->executor, manager->error_handler);
		// ft_printf(STDOUT_FILENO, "running_status: %d\n", running_status);
		free_cmd_list(&manager->executor->cmds);
		free(manager->parser->line);
	}
	free_shell_manager(manager);
	exit(0);
}
