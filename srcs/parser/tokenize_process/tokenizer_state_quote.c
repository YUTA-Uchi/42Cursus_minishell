/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_state_quote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:45:28 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 16:34:00 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	tokenize_state_in_single_quote(t_state *state, t_list **head, char c)
{
	if (!append_char_to_token(head, c))
		return (print_error_with_status(MALLOCF, 0), false);
	if (c == '\'')
	{
		*state = STATE_WORD;
	}
	return (true);
}

bool	tokenize_state_in_double_quote(t_state *state, t_list **head, char c)
{
	if (!append_char_to_token(head, c))
		return (print_error_with_status(MALLOCF, 0), false);
	if (c == '\"')
		*state = STATE_WORD;
	return (true);
}

bool	is_quote_end(char c, t_state state)
{
	return ((state == STATE_IN_SINGLE_QUOTE && c == '\'') \
			|| (state == STATE_IN_DOUBLE_QUOTE && c == '\"'));
}
