/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:33:22 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 14:55:51 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "command_processor.h"
#include "signals.h"

bool	prepare_commands(t_executor **executor \
	, t_parser **parser, t_shell_state *sh_state)
{
	if (handle_pending_signals(sh_state))
	{
		free_parser(*parser);
		free_executor(*executor);
		return (false);
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

void	cleanup_after_execution(t_executor *executor, t_shell_state *sh_state \
			, int status)
{
	sh_state->last_status = status;
	repair_std_io(executor);
	free_executor(executor);
}

bool	process_command_line(t_shell_state *sh_state)
{
	t_parser	*parser;
	t_executor	*executor;
	int			status;

	parser = create_parser(sh_state);
	if (!parser)
		return (false);
	executor = create_executor();
	if (!executor)
	{
		free_parser(parser);
		return (false);
	}
	if (!prepare_commands(&executor, &parser, sh_state))
	{
		return (true);
	}
	status = executor->execute(executor, sh_state);
	cleanup_after_execution(executor, sh_state, status);
	return (true);
}
