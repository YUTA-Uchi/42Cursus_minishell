/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 13:06:12 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static char	*ft_readline(t_error_handler *error_handler, const char *prompt)
{
	char	*line;

	(void)error_handler;
	rl_outstream = stderr;
	line = readline(prompt);
	if (!line)
	{
		// TODO error handling
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}

t_parser	*create_parser(t_error_handler *error_handler)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->parse = parse;
	parser->line = ft_readline(error_handler, PROMPT);
	return (parser);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}
