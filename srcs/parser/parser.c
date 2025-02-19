/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 19:54:30 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current_cmd;
	t_token	*token;

	head = NULL;
	current_cmd = NULL;
	token = tokens;
	while (token)
	{
		if (!current_cmd)
		{
			current_cmd = create_new_cmd();
			if (!head)
				head = current_cmd;
			else
				append_cmd(&head, current_cmd);
		}
		if (token->type == TOKEN_PIPE)
		{
			current_cmd = NULL;
			token = token->next;
			continue ;
		}
		if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT)
		{
			if (token->next && token->next->type == TOKEN_WORD)
			{
				t_redirection *redir = create_new_redirection(token->type, token->next->value);
				append_redirection(&current_cmd->redirections, redir);
				token = token->next;
			}
		}
		else if (token->type == TOKEN_WORD)
		{
			if (!current_cmd->cmd_name)
			{
				current_cmd->cmd_name = strdup(token->value);
				add_arg(&current_cmd->args, strdup(token->value));
			}
			else
			{
				add_arg(&current_cmd->args, strdup(token->value));
			}
		}
		token = token->next;
	}
	return (head);
}


t_cmd	*parse(t_parser *parser)
{
	char	*line;
	t_cmd	*cmd;

	line = parser->line;
	cmd = parse_tokens(tokenize_line(line));
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
