/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes_filter.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:02:23 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 18:02:42 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander_filter.h"

static bool	remove_quotes_from_token(t_token *token)
{
	char	*new_value;
	int		i;
	int		j;
	char	quote_type;

	new_value = malloc((token->len + 1) * sizeof(char));
	if (!new_value)
		return (print_error(MALLOCF));
	i = 0;
	j = 0;
	quote_type = '\0';
	while (token->value[i])
	{
		if ((token->value[i] == '\'' || token->value[i] == '\"') && \
				quote_type == '\0')
			quote_type = token->value[i];
		else if (token->value[i] == quote_type)
			quote_type = '\0';
		else
			new_value[j++] = token->value[i];
		i++;
	}
	new_value[j] = '\0';
	free(token->value);
	token->value = new_value;
	token->len = j;
	token->capacity = j + 1;
	return (true);
}

bool	remove_quotes(t_list **tokens, t_shell_state *state)
{
	t_list	*current;
	t_token	*token;

	(void)state;
	current = *tokens;
	while (current)
	{
		token = (t_token *)(current->content);
		if (token->type == TOKEN_WORD || \
			token->type == TOKEN_HEREDOC_DELIMITER)
		{
			if (!remove_quotes_from_token(token))
				return (false);
		}
		current = current->next;
	}
	return (true);
}