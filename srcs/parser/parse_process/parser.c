/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 16:04:35 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

// void	print_tokens(t_list *token_list)
// {
// 	t_list	*current;

// 	current = token_list;
// 	while (current)
// 	{
// 		ft_printf(STDERR_FILENO, "type: %d, value: %s\n"
// 			, ((t_token *)(current->content))->type
// 			, ((t_token *)(current->content))->value);
// 		current = current->next;
// 	}
// }

t_list	*parse(t_parser *parser, t_shell_state *shell_state)
{
	t_list	*cmd_list;
	t_list	*token_list;

	if (!parser || !parser->line)
		return (NULL);
	token_list = tokenize_line(parser->line);
	if (!token_list)
		return (NULL);
	if (!expansion(&token_list, shell_state))
	{
		ft_lstclear(&token_list, free_token);
		return (NULL);
	}
	cmd_list = parse_tokens(token_list, shell_state);
	ft_lstclear(&token_list, free_token);
	return (cmd_list);
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
	if (!parser)
		return ;
	if (parser->line)
		free(parser->line);
	free(parser);
}
