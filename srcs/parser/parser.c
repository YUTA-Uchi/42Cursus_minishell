/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:06:37 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static char	*ft_readline(t_shell_state *sh_state, const char *prompt)
{
	char	*line;

	(void)sh_state;
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

t_parser	*create_parser(t_shell_state *sh_state)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->parse = parse;
	parser->line = ft_readline(sh_state, PROMPT);
	return (parser);
}

void	free_parser(t_parser *parser)
{
	if (parser->line)
		free(parser->line);
	free(parser);
}
