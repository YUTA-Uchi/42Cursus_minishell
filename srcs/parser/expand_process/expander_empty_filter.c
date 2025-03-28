/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_empty_filter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:05:44 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 18:05:54 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander_filter.h"

bool	remove_empty_tokens(t_list **tokens, t_shell_state *state)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	(void)state;
	current = *tokens;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)(current->content);
		if (token->type == TOKEN_WORD && is_empty_token(token))
		{
			remove_token_node(tokens, current, prev);
			current = next;
		}
		else
		{
			prev = current;
			current = next;
		}
	}
	return (true);
}
