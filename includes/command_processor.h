/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:25 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 14:51:10 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_PROCESSOR_H
# define COMMAND_PROCESSOR_H

# include "executor.h"
# include "parser.h"

bool	prepare_commands(t_executor **executor \
	, t_parser **parser, t_shell_state *sh_state);
void	cleanup_after_execution(t_executor *executor, t_shell_state *sh_state \
	, int status);
bool	process_command_line(t_shell_state *sh_state);

#endif