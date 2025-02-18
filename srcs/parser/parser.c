/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 17:36:21 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_cmd	*parse(t_parser *parser)
{
	char	*line;
	t_cmd	*cmd;

	line = parser->line;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit(1);
	cmd->args = ft_split(line, ' ');
	if (!cmd->args)
		exit(1);
	cmd->cmd_name = cmd->args[0];
	if (!cmd->cmd_name)
		exit(1);
	cmd->pid = 0;
	return (cmd);
}

t_parser	*create_parser(void)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->line = NULL;
	parser->order = parse;
	return (parser);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}
