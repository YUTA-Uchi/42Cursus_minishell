/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 12:36:17 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_manager.h"

int	main(void)
{
	t_shell_manager	*shell_manager;

	shell_manager = create_shell_manager();
	if (!shell_manager)
		exit(1);
	while (1)
	{
		set_line(shell_manager);
		if (!shell_manager->parser->line)
			continue ;
		set_cmds(shell_manager);
		// if (!shell_manager->executor->cmds)
		// 	// TODO error handling parser error;
		shell_manager->executor->excute(shell_manager->executor);
		free(shell_manager->parser->line);
	}
	free_shell_manager(shell_manager);
	exit(0);
}
