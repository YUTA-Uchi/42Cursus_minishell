/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:35:28 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 17:26:50 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

extern volatile sig_atomic_t	g_signal;

static void	handle_eof(t_shell_state *sh_state)
{
	sh_state->running = false;
	sh_state->last_status = E_SUCCESS;
	if (sh_state->is_interactive)
		ft_printf(STDOUT_FILENO, "exit\n");
}

static char	*handle_sigint(char *line, t_shell_state *sh_state)
{
	free(line);
	sh_state->last_status = SIGINT | E_SIGTERM;
	g_signal = 0;
	return (NULL);
}

static void	add_line_to_history(char *line)
{
	add_history(line);
}

char	*ft_readline(t_shell_state *sh_state, const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (!line)
	{
		handle_eof(sh_state);
		return (NULL);
	}
	if (g_signal == SIGINT)
		return (handle_sigint(line, sh_state));
	if (sh_state->is_interactive && *line)
		add_line_to_history(line);
	return (line);
}
