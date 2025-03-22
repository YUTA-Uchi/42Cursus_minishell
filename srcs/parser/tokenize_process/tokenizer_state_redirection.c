/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_state_redirection.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:22:41 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/22 14:31:17 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	handle_redirection_word(t_list **head, char c, t_state *state)
{
	t_list	*token;

	token = create_token(get_token_type(c), c);
	if (!token)
		return (false);
	ft_lstadd_back(head, token);
	if (c == '\'')
		*state = STATE_IN_SINGLE_QUOTE;
	else if (c == '\"')
		*state = STATE_IN_DOUBLE_QUOTE;
	else if (!isspace(c))
		*state = STATE_WORD;
	return (true);
}

bool	tokenize_state_in_redir_in(t_state *state, t_list **head, char c)
{
	t_list	*token;

	if (c == '<')
	{
		token = ft_lstlast(*head);
		((t_token *)(token->content))->type = TOKEN_REDIR_HEREDOC;
		return (true);
	}
	if (c == '>')
	{
		print_const_error(SYNERR_NEAR_RIN, 0);
		return (false);
	}
	if (isspace(c))
	{
		*state = STATE_NONE;
		return (true);
	}
	return (handle_redirection_word(head, c, state));
}

bool	tokenize_state_in_redir_out(t_state *state, t_list **head, char c)
{
	t_list	*token;

	if (c == '>')
	{
		token = ft_lstlast(*head);
		((t_token *)(token->content))->type = TOKEN_REDIR_APPEND;
		return (true);
	}
	if (c == '<')
	{
		print_const_error(SYNERR_NEAR_ROUT, 0);
		return (false);
	}
	if (isspace(c))
	{
		*state = STATE_NONE;
		return (true);
	}
	return (handle_redirection_word(head, c, state));
}
