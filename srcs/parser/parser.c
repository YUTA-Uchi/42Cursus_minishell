/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 18:57:12 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

bool	add_arg(char ***args, char *arg)
{
	char	**new_args;
	int		i;

	if (!args || !arg)
		return (false);
	i = 0;
	while ((*args)[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (false);
	i = 0;
	while ((*args)[i])
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[i] = arg;
	new_args[i + 1] = NULL;
	free(*args);
	*args = new_args;
	return (true);
}

t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else
		return (REDIR_APPEND);
}

t_list	*parse_tokens(t_list *tokens, t_error_handler *err_handler)
{
	t_list	*head;
	t_list	*current_cmd;
	t_token	*token_content;
	t_cmd	*cmd_content;

	head = NULL;
	current_cmd = NULL;
	if (!tokens)
		return (NULL);
	while (tokens)
	{
		token_content = (t_token *)(tokens->content);
		if (!current_cmd)
		{
			current_cmd = create_cmd();
			if (!head)
				head = current_cmd;
			else
				ft_lstadd_back(&head, current_cmd);
		}
		if (token_content->type == TOKEN_PIPE)
		{
			current_cmd = NULL;
			tokens = tokens->next;
			continue ;
		}
		cmd_content = (t_cmd *)(current_cmd->content);
		if (token_content->type == TOKEN_REDIR_IN || token_content->type == TOKEN_REDIR_OUT)
		{
			if (tokens->next && ((t_token *)(tokens->next))->type == TOKEN_WORD)
			{
				ft_lstadd_back(&(cmd_content->redirections) \
				, create_redirection(((t_token *)(tokens->next))->value \
				, get_redir_type(token_content->type)));
				tokens = tokens->next;
			}
		}
		else if (token_content->type == TOKEN_WORD)
		{
			if (!cmd_content->cmd_name)
				cmd_content->cmd_name = ft_strdup(token_content->value);
			if (!cmd_content->cmd_name || !add_arg(&(cmd_content->args), ft_strdup(token_content->value)))
				return (set_error(err_handler, E_GENERAL_ERR, "malloc failed"), NULL);
		}
		tokens = tokens->next;
	}
	return (head);
}


t_list	*parse(t_parser *parser, t_error_handler *error_handler)
{
	char	*line;
	t_list	*cmd;
	t_list	*token_list;

	line = parser->line;
	token_list = tokenize_line(line);
	cmd = parse_tokens(token_list, error_handler);
	return (cmd);
}

t_parser	*create_parser(void)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->line = NULL;
	parser->parse = parse;
	return (parser);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}
