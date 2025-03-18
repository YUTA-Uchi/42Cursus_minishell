/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:10:06 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 16:51:03 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	finalize_expand(t_expand *expand, t_list *env_list)
{
	if (expand->state == EXPAND_IN_ENV)
	{
		if (!set_env_value_to_str(expand, env_list))
			return (false);
	}
	return (true);
}

static t_list	*create_new_expanded_node(t_expand *expand)
{
	t_list	*new_token_node;
	t_token	*token_content;

	new_token_node = create_token(TOKEN_WORD, '\0');
	if (!new_token_node || !expand->value || !new_token_node->content)
		return (NULL);
	token_content = (t_token *)(new_token_node->content);
	free(token_content->value);
	token_content->value = ft_strdup(expand->value);
	if (!token_content->value)
		return (NULL);
	token_content->len = expand->len;
	token_content->capacity = expand->capacity;
	return (new_token_node);
}

static bool	replace_token_node_with_expanded_node(t_list **current \
											, t_list *prev, t_expand **expand)
{
	t_list		*new_token_node;

	new_token_node = create_new_expanded_node(*expand);
	if (!new_token_node)
		return (print_const_error(MALLOCF, 0), false);
	free_expand(*expand);
	if (prev)
		prev->next = new_token_node;
	new_token_node->next = (*current)->next;
	ft_lstdelone((*current), free_token);
	*current = new_token_node;
	return (true);
}

static bool	expand_word(t_list **current_node, t_list *prev_node \
						, t_shell_state *shell_state)
{
	t_expand		*expand;
	t_token			*token_content;
	int				i;

	token_content = (t_token *)((*current_node)->content);
	expand = create_expand(shell_state);
	i = 0;
	while (token_content->value[i])
	{
		if (!handle_expand_state(expand, token_content->value[i++] \
			, shell_state->env_list))
			return (print_const_error(MALLOCF, 0), false);
	}
	if (!finalize_expand(expand, shell_state->env_list))
		return (print_const_error(MALLOCF, 0), false);
	if (!replace_token_node_with_expanded_node(current_node \
											, prev_node, &expand))
		return (false);
	return (true);
}

bool	expansion(t_list **token_list, t_shell_state *shell_state)
{
	t_list	*current_token_node;
	t_list	*prev_token_node;
	t_list	*next_token_node;
	t_token	*token_content;

	current_token_node = *token_list;
	prev_token_node = NULL;
	while (current_token_node)
	{
		next_token_node = current_token_node->next;
		token_content = (t_token *)(current_token_node->content);
		if (token_content->type == TOKEN_WORD)
		{
			if (!expand_word(&current_token_node, prev_token_node, shell_state))
				return (false);
		}
		if (!prev_token_node)
			*token_list = current_token_node;
		prev_token_node = current_token_node;
		current_token_node = next_token_node;
	}
	return (true);
}
