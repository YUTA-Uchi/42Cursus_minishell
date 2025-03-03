/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:22:57 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/02 15:38:29 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

t_list	*create_token(t_token_type type, const char value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value == '\0')
		token->len = 0;
	else
		token->len = 1;
	token->capacity = INITIAL_TOKEN_BUF_SIZE;
	token->value = ft_calloc(token->capacity, sizeof(char));
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->value[0] = value;
	return (ft_lstnew(token));
}

bool	append_char_to_token(t_state *state, t_list **head, char c)
{
	char	*new_buffer;
	int		i;
	t_list	*current;
	t_token	*token_content;
	t_list	*token;

	current = ft_lstlast(*head);
	if (*state == STATE_NONE)
	{
		// ft_printf(STDOUT_FILENO, "append_char:%c\n", c);
		if (c == '\'' || c == '\"')
		{
			token = create_token(TOKEN_WORD, '\0');
			if (!token)
				return (false);
			ft_lstadd_back(head, token);
		}
		else
		{
			token = create_token(TOKEN_WORD, c);
			if (!token)
				return (false);
			ft_lstadd_back(head, token);
		}
		return (true);
	}
	token_content = (t_token *)(current->content);
	if (token_content->len + 1 >= token_content->capacity)
	{
		token_content->capacity *= 2;
		new_buffer = malloc(token_content->capacity);
		if (!new_buffer)
			return (false);
		i = 0;
		while (i < token_content->len)
		{
			new_buffer[i] = token_content->value[i];
			i++;
		}
		free(token_content->value);
		token_content->value = new_buffer;
	}
	token_content->value[token_content->len] = c;
	token_content->len++;
	token_content->value[token_content->len] = '\0';
	// ft_printf(STDOUT_FILENO, "append_char:%c\n", c);
	return (true);
}

bool	tokenize_state_none(t_state *state, t_list **head, char c)
{
	t_list	*token;

	// ft_printf(STDOUT_FILENO, "tokenize_state_none\n");
	if (isspace(c))
	{
		*state = STATE_NONE;
	}
	else if (c == '<' || c == '>' || c == '|')
	{
		token = create_token(get_token_type(c), c);
		if (!token)
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
		ft_lstadd_back(head, token);
	}
	else
	{
		if (!append_char_to_token(state, head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
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
	t_list	*token;

	// ft_printf(STDOUT_FILENO, "tokenize_state_word\n");
	if (isspace(c))
	{
		*state = STATE_NONE;
	}
	else if (c == '<' || c == '>' || c == '|')
	{
		token = create_token(get_token_type(c), c);
		if (!token)
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
		ft_lstadd_back(head, token);
		*state = STATE_NONE;
	}
	else if (c == '\'')
		*state = STATE_IN_SINGLE_QUOTE;
	else if (c == '\"')
		*state = STATE_IN_DOUBLE_QUOTE;
	else
	{
		if (!append_char_to_token(state, head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	}
	return (true);
}

bool	tokenize_state_in_single_quote(t_state *state, t_list **head, char c)
{
	// ft_printf(STDOUT_FILENO, "tokenize_state_in_single_quote\n");
	if (c == '\'')
	{
		*state = STATE_WORD;
	}
	else
	{
		if (!append_char_to_token(state, head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	}
	return (true);
}

bool	tokenize_state_in_double_quote(t_state *state, t_list **head, char c)
{
	// ft_printf(STDOUT_FILENO, "tokenize_state_in_double_quote\n");
	if (c == '\"')
	{
		*state = STATE_WORD;
	}
	else
	{
		if (!append_char_to_token(state, head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	}
	return (true);
}

void	free_token(void *token)
{
	t_token	*token_ptr;

	if (!token)
		return ;
	token_ptr = (t_token *)token;
	if (token_ptr->value)
		free(token_ptr->value);
	free(token_ptr);
}

t_list	*tokenize_line(const char *line)
{
	t_list	*head;
	t_state	state;
	int		i;

	head = NULL;
	state = STATE_NONE;
	i = 0;
	while (line[i])
	{
		// ft_printf(STDOUT_FILENO, "line[%d]:%c\n", i, line[i]);
		if (state == STATE_NONE)
		{
			if (!tokenize_state_none(&state, &head, line[i]))
				return (ft_lstclear(&head, free_token), NULL);
		}
		else if (state == STATE_WORD)
		{
			if (!tokenize_state_word(&state, &head, line[i]))
				return (ft_lstclear(&head, free_token), NULL);
		}
		else if (state == STATE_IN_SINGLE_QUOTE)
		{
			if (!tokenize_state_in_single_quote(&state, &head, line[i]))
				return (ft_lstclear(&head, free_token), NULL);
		}
		else if (state == STATE_IN_DOUBLE_QUOTE)
		{
			if (!tokenize_state_in_double_quote(&state, &head, line[i]))
				return (ft_lstclear(&head, free_token), NULL);
		}
		// ft_printf(STDOUT_FILENO, "state:%d\n", state);
		i++;
	}
	if (state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE)
	{
		ft_printf(STDERR_FILENO, "minishell: Unclosed quote detected.\n");
		return (ft_lstclear(&head, free_token), NULL);
	}
	return (head);
}

