/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:42:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 12:58:54 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else
		return (REDIR_HEREDOC);
}

static const char	*get_token_str(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return ("<");
	else if (token_type == TOKEN_REDIR_OUT)
		return (">");
	else if (token_type == TOKEN_REDIR_APPEND)
		return (">>");
	else if (token_type == TOKEN_REDIR_HEREDOC)
		return ("<<");
	else
		return ("|");
}

bool	parse_redirection(t_list **token_list, t_cmd *cmd_content)
{
	t_token			*token_content;
	t_token			*next_token_content;
	t_list			*redirection;
	t_redir_type	redir_type;

	token_content = (t_token *)((*token_list)->content);
	if (token_content->type < TOKEN_REDIR_IN)
		return (true);
	redir_type = get_redir_type(token_content->type);
	if (!(*token_list)->next)
		return (print_error(SYNERR_NEWLINE));
	next_token_content = (t_token *)((*token_list)->next->content);
	if (!(next_token_content->type == TOKEN_WORD \
		|| next_token_content->type == TOKEN_HEREDOC_DELIMITER))
		return (print_error_with_value(SYNERR_UNEXPECTED \
								, get_token_str(next_token_content->type)));
	redirection = create_redirection(next_token_content->value \
									, redir_type);
	if (!redirection)
		return (print_error(MALLOCF));
	ft_lstadd_back(&(cmd_content->redirections), redirection);
	*token_list = (*token_list)->next;
	return (true);
}
