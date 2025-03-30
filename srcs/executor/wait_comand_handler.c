/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_comand_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:10:54 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/30 18:24:28 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"

static void	display_signal_message(int signal, bool *sigint_displayed \
	, bool *sigquit_displayed)
{
	if (signal == SIGINT && !(*sigint_displayed))
	{
		ft_printf(STDERR_FILENO, "\n");
		*sigint_displayed = true;
	}
	else if (signal == SIGQUIT && !(*sigquit_displayed))
	{
		ft_printf(STDERR_FILENO, "Quit (core dumped)\n");
		*sigquit_displayed = true;
	}
	else if (signal == SIGPIPE)
	{
		ft_printf(STDERR_FILENO, "Broken pipe\n");
	}
}

static int	handle_exit_status(int status, bool *sigint_displayed \
			, bool *sigquit_displayed)
{
	int	signal;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		display_signal_message(signal, sigint_displayed, sigquit_displayed);
		return (signal + E_SIGTERM);
	}
	return (E_GENERAL_ERR);
}

static int	wait_single_command(t_cmd *cmd_content, bool *sigint_displayed \
, bool *sigquit_displayed)
{
	int		status;
	pid_t	wait_result;

	while (1)
	{
		if (cmd_content->pid == -1 && g_signal != 0)
			return (g_signal | E_SIGTERM);
		if (cmd_content->pid == -1)
			return (E_GENERAL_ERR);
		wait_result = waitpid(cmd_content->pid, &status, 0);
		if (wait_result != -1 || errno != EINTR)
			break ;
	}
	if (wait_result == -1)
		return (E_GENERAL_ERR);
	return (handle_exit_status(status, sigint_displayed, sigquit_displayed));
}

int	wait_all_children(t_list *cmd_list)
{
	t_list	*current;
	bool	sigint_displayed;
	bool	sigquit_displayed;
	int		last_status;
	t_cmd	*cmd_content;

	sigint_displayed = false;
	sigquit_displayed = false;
	last_status = 0;
	current = cmd_list;
	while (current)
	{
		cmd_content = (t_cmd *)(current->content);
		last_status = wait_single_command(cmd_content, &sigint_displayed \
										, &sigquit_displayed);
		current = current->next;
	}
	if (errno != 0 && errno != ECHILD && errno != EINTR)
	{
		print_strerror("waitpid");
		return (E_GENERAL_ERR);
	}
	return (last_status);
}
