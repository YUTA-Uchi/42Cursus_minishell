/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_state_handler_normal.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 15:40:19 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	tokenize_state_none(t_state *state, t_list **head, char c)
{
	if (isspace(c))
	{
		*state = STATE_NONE;
	}
	else if (c == '<' || c == '>' || c == '|')
	{
		ft_lstadd_back(head, create_token(get_token_type(c), c));
		if (c == '<')
			*state = STATE_REDIR_IN;
		else if (c == '>')
			*state = STATE_REDIR_OUT;
	}
	else
	{
		ft_lstadd_back(head, create_token(get_token_type(c), c));
		if (c == '\'')
			*state = STATE_IN_SINGLE_QUOTE;
		else if (c == '\"')
			*state = STATE_IN_DOUBLE_QUOTE;
		else if (!isspace(c))
			*state = STATE_WORD;
	}
	return (true);
}

bool	tokenize_state_word(t_state *state, t_list **head, char c)
{
	if (isspace(c))
	{
		*state = STATE_NONE;
	}
	else if (c == '<' || c == '>' || c == '|')
	{
		ft_lstadd_back(head, create_token(get_token_type(c), c));
		*state = STATE_NONE;
		if (c == '<')
			*state = STATE_REDIR_IN;
		else if (c == '>')
			*state = STATE_REDIR_OUT;
	}
	else
	{
		if (!append_char_to_token(head, c))
			return (print_const_error(MALLOCF, 0), false);
		if (c == '\'')
			*state = STATE_IN_SINGLE_QUOTE;
		else if (c == '\"')
			*state = STATE_IN_DOUBLE_QUOTE;
	}
	return (true);
}

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
