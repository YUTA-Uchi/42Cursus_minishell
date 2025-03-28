/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable_filter.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:01:20 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 18:03:20 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander_filter.h"

static bool	expand_token_variables(t_list **current, t_list *prev, \
						t_shell_state *state)
{
	t_expand	*expand_ctx;
	t_token		*token_content;
	int			i;

	token_content = (t_token *)((*current)->content);
	expand_ctx = create_expand(state);
	if (!expand_ctx)
		return (print_error(MALLOCF));
	i = 0;
	while (token_content->value[i])
	{
		if (!dispatch_expand_state_handler(expand_ctx \
				, token_content->value[i++], state->env_list))
			return (free_expand(expand_ctx), print_error(MALLOCF));
	}
	if (!resolve_pending_expansion(expand_ctx, state->env_list))
		return (free_expand(expand_ctx), print_error(MALLOCF));
	if (!replace_with_expanded_token(current, prev, &expand_ctx))
		return (false);
	return (true);
}

bool	expand_variables(t_list **tokens, t_shell_state *state)
{
	t_list	*current;
	t_list	*prev;
	t_token	*token;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		token = (t_token *)(current->content);
		if (token->type == TOKEN_WORD)
		{
			if (!expand_token_variables(&current, prev, state))
				return (false);
		}
		prev = current;
		current = current->next;
	}
	return (true);
}
