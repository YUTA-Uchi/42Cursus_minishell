/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:10:06 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/22 18:19:58 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	expand_word(t_list **current_node, t_list *prev_node \
						, t_shell_state *shell_state)
{
	t_expand		*expand;
	t_token			*token_content;
	int				i;

	token_content = (t_token *)((*current_node)->content);
	expand = create_expand(shell_state);
	if (!expand)
		return (print_const_error(MALLOCF, 0), false);
	i = 0;
	while (token_content->value[i])
	{
		if (!handle_expand_state(expand, token_content->value[i++] \
			, shell_state->env_list))
			return (free_expand(expand), print_const_error(MALLOCF, 0), false);
	}
	if (!finalize_expand(expand, shell_state->env_list))
		return (free_expand(expand), print_const_error(MALLOCF, 0), false);
	if (!replace_with_expanded_token(current_node, prev_node, &expand))
		return (false);
	return (true);
}

static bool	process_expanded_token(t_list **token_list, t_list **current,
	t_list *prev, t_shell_state *shell_state)
{
	t_token	*token_content;
	t_list	*next_token;

	token_content = (t_token *)((*current)->content);
	next_token = (*current)->next;
	if (token_content->type == TOKEN_WORD)
	{
		if (!expand_word(current, prev, shell_state))
			return (false);

		token_content = (t_token *)((*current)->content);
		if (is_empty_token(token_content))
		{
			remove_token_node(token_list, *current, prev);
			*current = next_token;
			return (true);
		}
	}
	return (true);
}

bool	expansion(t_list **token_list, t_shell_state *shell_state)
{
	t_list	*current_token_node;
	t_list	*prev_token_node;
	t_list	*next_token_node;

	current_token_node = *token_list;
	prev_token_node = NULL;
	while (current_token_node)
	{
		next_token_node = current_token_node->next;
		if (!process_expanded_token(token_list, &current_token_node \
									, prev_token_node, shell_state))
			return (false);
		if (current_token_node == next_token_node)
			continue ;
		if (!prev_token_node)
			*token_list = current_token_node;
		prev_token_node = current_token_node;
		current_token_node = next_token_node;
	}
	return (true);
}
