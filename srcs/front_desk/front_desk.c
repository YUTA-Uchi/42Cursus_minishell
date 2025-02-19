/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front_desk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:11:09 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 12:13:46 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "front_desk.h"

void	set_line(t_front_desk *front_desk)
{
	char	*line;

	line = front_desk->listen(front_desk, PROMPT);
	front_desk->parser->line = line;
}

void	set_cmds(t_front_desk *front_desk)
{
	front_desk->executor->cmds = front_desk->parser->order(front_desk->parser);
}

static char	*ft_readline(t_front_desk *self, const char *prompt)
{
	char	*line;

	rl_outstream = stderr;
	line = readline(prompt);
	if (!line)
	{
		// TODO error handling
		self->error_handler->error = E_GENERAL_ERR;
		self->error_handler->msg = "readline: ";
		fatal_error("ft_readline", strerror(errno));
	}
	if (*line)
		add_history(line);
	return (line);
}

static void	initialize_front_desk(t_front_desk *front_desk)
{
	front_desk->parser = NULL;
	front_desk->executor = NULL;
	front_desk->error_handler = NULL;
	front_desk->listen = ft_readline;
}

t_front_desk	*create_front_desk(void)
{
	t_front_desk	*front_desk;

	front_desk = malloc(sizeof(t_front_desk));
	if (!front_desk)
		return (NULL);
	initialize_front_desk(front_desk);
	front_desk->parser = create_parser();
	if (!front_desk->parser)
		return (free_front_desk(front_desk), NULL);
	front_desk->executor = create_executor();
	if (!front_desk->executor)
		return (free_front_desk(front_desk), NULL);
	front_desk->error_handler = create_error_handler();
	if (!front_desk->error_handler)
		return (free_front_desk(front_desk), NULL);
	return (front_desk);
}

void	free_front_desk(t_front_desk *front_desk)
{
	if (front_desk->parser)
		free_parser(front_desk->parser);
	if (front_desk->executor)
		free_executor(front_desk->executor);
	if (front_desk->error_handler)
		free_error_handler(front_desk->error_handler);
	free(front_desk);
}
