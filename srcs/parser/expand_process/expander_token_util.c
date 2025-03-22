/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_token_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:18:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/22 18:09:07 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

bool	is_empty_token(t_token *token)
{
	return (!token->value || token->value[0] == '\0');
}

void	remove_token_node(t_list **token_list, t_list *to_remove \
							, t_list *prev)
{
	if (!prev)
		*token_list = to_remove->next;
	else
		prev->next = to_remove->next;
	ft_lstdelone(to_remove, free_token);
}

bool	finalize_expand(t_expand *expand, t_list *env_list)
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

bool	replace_with_expanded_token(t_list **current \
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
