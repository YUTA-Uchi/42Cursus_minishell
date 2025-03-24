/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_token_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:18:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 16:34:00 by yuuchiya         ###   ########.fr       */
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

bool	resolve_pending_expansion(t_expand *expand_ctx, t_list *env_list)
{
	if (expand_ctx->state == EXPAND_IN_ENV)
	{
		if (!set_env_value_to_str(expand_ctx, env_list))
			return (false);
	}
	return (true);
}

static t_list	*create_new_expanded_node(t_expand *expand_ctx)
{
	t_list	*new_token_node;
	t_token	*token_content;

	new_token_node = create_token(TOKEN_WORD, '\0');
	if (!new_token_node || !expand_ctx->value || !new_token_node->content)
		return (NULL);
	token_content = (t_token *)(new_token_node->content);
	free(token_content->value);
	token_content->value = ft_strdup(expand_ctx->value);
	if (!token_content->value)
		return (NULL);
	token_content->len = expand_ctx->len;
	token_content->capacity = expand_ctx->capacity;
	return (new_token_node);
}

bool	replace_with_expanded_token(t_list **current \
										, t_list *prev, t_expand **expand_ctx)
{
	t_list		*new_token_node;

	new_token_node = create_new_expanded_node(*expand_ctx);
	if (!new_token_node)
		return (print_error_with_status(MALLOCF, 0), false);
	free_expand(*expand_ctx);
	if (prev)
		prev->next = new_token_node;
	new_token_node->next = (*current)->next;
	ft_lstdelone((*current), free_token);
	*current = new_token_node;
	return (true);
}
