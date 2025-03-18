/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 17:43:37 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

extern volatile sig_atomic_t	g_signal;

t_list	*parse(t_parser *parser, t_shell_state *shell_state)
{
	t_list	*cmd_list;
	t_list	*token_list;

	token_list = tokenize_line(parser->line);
	if (!token_list)
		return (NULL);
	if (!expansion(&token_list, shell_state))
		return (ft_lstclear(&token_list, free_token), NULL);
	cmd_list = parse_tokens(token_list, shell_state);
	ft_lstclear(&token_list, free_token);
	if (!cmd_list)
		return (NULL);
	return (cmd_list);
}

static char	*ft_readline(t_shell_state *sh_state, const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (!line)
	{
		sh_state->running = false;
		sh_state->last_status = E_SUCCESS;
		ft_printf(STDOUT_FILENO, "exit\n");
		return (NULL);
	}
	if (g_signal == SIGINT)
	{
		free(line);
		sh_state->last_status = SIGINT | 128;
		g_signal = 0;
		return (NULL);
	}
	if (*line)
	{
		add_history(line);
	}
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
	if (!parser->line)
	{
		free(parser);
		return (NULL);
	}
	return (parser);
}

void	free_parser(t_parser *parser)
{
	if (parser->line)
		free(parser->line);
	free(parser);
}
