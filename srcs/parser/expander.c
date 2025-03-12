/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:10:06 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:16:57 by yuuchiya         ###   ########.fr       */
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

static t_list	*set_token_value(t_list *token, t_expand *expand)
{
	t_token	*token_content;

	if (!token || !expand->value || !token->content)
		return (NULL);
	token_content = (t_token *)(token->content);
	free(token_content->value);
	token_content->value = ft_strdup(expand->value);
	if (!token_content->value)
		return (NULL);
	token_content->len = expand->len;
	token_content->capacity = expand->capacity;
	return (token);
}

static bool	expand_word(t_list **current, t_list *prev \
						, t_shell_state *shell_state)
{
	t_expand		*expand;
	t_token			*token;
	t_list			*new_token;
	int				i;

	token = (t_token *)((*current)->content);
	expand = create_expand(shell_state);
	i = 0;
	while (token->value[i])
	{
		if (!handle_expand_state(expand, token->value[i++], shell_state->env_list))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer1", "malloc failed"), false);
	}
	if (!finalize_expand(expand, shell_state->env_list))
		return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer2", "malloc failed"), false);
	new_token = set_token_value(create_token(TOKEN_WORD, '\0'), expand);
	if (!new_token)
		return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer3", "malloc failed"), false);
	free_expand(expand);
	if (prev)
		prev->next = new_token;
	new_token->next = (*current)->next;
	ft_lstdelone((*current), free_token);
	*current = new_token;
	return (true);
}

bool	expansion(t_list **token_list, t_shell_state *shell_state)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = *token_list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)(current->content);
		if (token->type == TOKEN_WORD)
		{
			if (!expand_word(&current, prev, shell_state))
				return (false);
		}
		if (!prev)
			*token_list = current;
		prev = current;
		current = next;
	}
	return (true);
}
