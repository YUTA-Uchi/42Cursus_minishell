/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_state_handler_normal.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/22 14:19:36 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	is_token_delimiter(char c)
{
	return (c == '<' || c == '>' || c == '|' || isspace(c));
}

static bool	add_new_token(t_list **head, char c, t_state *state)
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
	else if (c == '<')
		*state = STATE_REDIR_IN;
	else if (c == '>')
		*state = STATE_REDIR_OUT;
	else if (!isspace(c))
		*state = STATE_WORD;
	return (true);
}

bool	tokenize_state_none(t_state *state, t_list **head, char c)
{
	if (isspace(c))
	{
		*state = STATE_NONE;
		return (true);
	}
	if (!add_new_token(head, c, state))
		return (print_const_error(MALLOCF, 0), false);
	return (true);
}

bool	tokenize_state_word(t_state *state, t_list **head, char c)
{
	if (isspace(c))
	{
		*state = STATE_NONE;
		return (true);
	}
	else if (is_token_delimiter(c))
	{
		if (!add_new_token(head, c, state))
			return (print_const_error(MALLOCF, 0), false);
		return (true);
	}
	if (!append_char_to_token(head, c))
		return (print_const_error(MALLOCF, 0), false);
	if (c == '\'')
		*state = STATE_IN_SINGLE_QUOTE;
	else if (c == '\"')
		*state = STATE_IN_DOUBLE_QUOTE;
	return (true);
}
