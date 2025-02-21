/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:11:09 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/21 14:34:17 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_manager.h"

// void	set_line(t_shell_manager *shell_manager)
// {
// 	char	*line;

// 	line = shell_manager->listen(shell_manager, PROMPT);
// 	shell_manager->parser->line = line;
// 	// ft_printf(STDOUT_FILENO, "line: %s\n", line);
// }

// void	set_cmds(t_shell_manager *shell_manager)
// {
// 	shell_manager->executor->cmds = shell_manager->parser->parse(shell_manager->parser, shell_manager->error_handler);
// }

// static void	initialize_shell_manager(t_shell_manager *shell_manager)
// {
// 	shell_manager->parser = NULL;
// 	shell_manager->executor = NULL;
// 	shell_manager->error_handler = NULL;
// 	shell_manager->listen = ft_readline;
// }

// t_shell_manager	*create_shell_manager(void)
// {
// 	t_shell_manager	*shell_manager;

// 	shell_manager = malloc(sizeof(t_shell_manager));
// 	if (!shell_manager)
// 		return (NULL);
// 	initialize_shell_manager(shell_manager);
// 	shell_manager->parser = create_parser();
// 	if (!shell_manager->parser)
// 		return (free_shell_manager(shell_manager), NULL);
// 	shell_manager->executor = create_executor();
// 	if (!shell_manager->executor)
// 		return (free_shell_manager(shell_manager), NULL);
// 	shell_manager->error_handler = create_error_handler();
// 	if (!shell_manager->error_handler)
// 		return (free_shell_manager(shell_manager), NULL);
// 	return (shell_manager);
// }

// void	free_shell_manager(t_shell_manager *shell_manager)
// {
// 	if (shell_manager->parser)
// 		free_parser(shell_manager->parser);
// 	if (shell_manager->executor)
// 		free_executor(shell_manager->executor);
// 	if (shell_manager->error_handler)
// 		free_error_handler(shell_manager->error_handler);
// 	free(shell_manager);
// }
