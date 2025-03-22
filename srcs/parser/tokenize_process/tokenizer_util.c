/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:05:38 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/22 14:07:36 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	append_char_to_token(t_list **head, char c)
{
	char	*new_buffer;
	t_list	*current;
	t_token	*token_content;

	current = ft_lstlast(*head);
	token_content = (t_token *)(current->content);
	if (token_content->len + 1 >= token_content->capacity)
	{
		token_content->capacity *= 2;
		new_buffer = malloc(token_content->capacity);
		if (!new_buffer)
			return (false);
		ft_strlcpy(new_buffer, token_content->value, token_content->len + 1);
		free(token_content->value);
		token_content->value = new_buffer;
	}
	token_content->value[token_content->len++] = c;
	token_content->value[token_content->len] = '\0';
	return (true);
}

t_token_type	get_token_type(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<')
		return (TOKEN_REDIR_IN);
	else if (c == '>')
		return (TOKEN_REDIR_OUT);
	else
		return (TOKEN_WORD);
}

bool	handle_tokenize_state(t_state *state, t_list **head, char c)
{
	const t_token_state_handler	handlers[6] = {\
		tokenize_state_none, \
		tokenize_state_word, \
		tokenize_state_in_single_quote, \
		tokenize_state_in_double_quote, \
		tokenize_state_in_redir_in, \
		tokenize_state_in_redir_out
	};

	return (handlers[*state](state, head, c));
}

t_list	*handle_token_error(t_list **token_list, const char *message)
{
	ft_printf(STDERR_FILENO, "minishell: %s\n", message);
	ft_lstclear(token_list, free_token);
	return (NULL);
}
