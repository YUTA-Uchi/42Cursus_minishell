/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_state_handler_meta_character.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:45:28 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/17 17:59:49 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	tokenize_state_in_single_quote(t_state *state, t_list **head, char c)
{
	if (!append_char_to_token(head, c))
		return (print_const_error(MALLOCF, 0), false);
	if (c == '\'')
	{
		*state = STATE_WORD;
	}
	return (true);
}

bool	tokenize_state_in_double_quote(t_state *state, t_list **head, char c)
{
	if (!append_char_to_token(head, c))
		return (print_const_error(MALLOCF, 0), false);
	if (c == '\"')
	{
		*state = STATE_WORD;
	}
	return (true);
}

bool	tokenize_state_in_redir_in(t_state *state, t_list **head, char c)
{
	t_list	*token;

	if (c == '<')
	{
		token = ft_lstlast(*head);
		((t_token *)(token->content))->type = TOKEN_REDIR_HEREDOC;
	}
	else if (c == '>')
		return (print_const_error(SYNERR_NEAR_RIN, 0), false);
	else if (isspace(c))
		*state = STATE_NONE;
	else
	{
		token = create_token(get_token_type(c), c);
		if (!token)
			return (print_const_error(MALLOCF, 0), false);
		ft_lstadd_back(head, token);
		if (c == '\'')
			*state = STATE_IN_SINGLE_QUOTE;
		else if (c == '\"')
			*state = STATE_IN_DOUBLE_QUOTE;
		else if (!isspace(c))
			*state = STATE_WORD;
	}
	return (true);
}

bool	tokenize_state_in_redir_out(t_state *state, t_list **head, char c)
{
	t_list	*token;

	if (c == '>')
	{
		token = ft_lstlast(*head);
		((t_token *)(token->content))->type = TOKEN_REDIR_APPEND;
	}
	else if (c == '<')
		return (print_const_error(SYNERR_NEAR_ROUT, 0), false);
	else if (isspace(c))
		*state = STATE_NONE;
	else
	{
		token = create_token(get_token_type(c), c);
		if (!token)
			return (print_const_error(MALLOCF, 0), false);
		ft_lstadd_back(head, token);
		if (c == '\'')
			*state = STATE_IN_SINGLE_QUOTE;
		else if (c == '\"')
			*state = STATE_IN_DOUBLE_QUOTE;
		else if (!isspace(c))
			*state = STATE_WORD;
	}
	return (true);
}
