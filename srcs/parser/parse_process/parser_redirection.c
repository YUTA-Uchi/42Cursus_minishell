/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:42:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 16:34:00 by yuuchiya         ###   ########.fr       */
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

bool	parse_redirection(t_list **token_list, t_cmd *cmd_content)
{
	t_token	*token_content;
	t_token	*next_token_content;
	t_list	*redirection;

	token_content = (t_token *)((*token_list)->content);
	if (token_content->type < TOKEN_REDIR_IN)
		return (true);
	if (!(*token_list)->next)
		return (print_error_with_status(SYNERR_NEWLINE, 0), false);
	next_token_content = (t_token *)((*token_list)->next->content);
	if (next_token_content->type != TOKEN_WORD)
		return (print_error_with_status(SYNERR, 0), false); // need to change
	redirection = create_redirection(next_token_content->value \
		, get_redir_type(token_content->type));
	if (!redirection)
		return (print_error_with_status(MALLOCF, 0), false);
	ft_lstadd_back(&(cmd_content->redirections), redirection);
	*token_list = (*token_list)->next;
	return (true);
}
