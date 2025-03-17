/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:22:57 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/17 17:55:13 by yuuchiya         ###   ########.fr       */
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
		if (!handle_tokenize_state(&state, &head, line[i]))
			return (ft_lstclear(&head, free_token), NULL);
		i++;
	}
	if (state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE)
	{
		ft_printf(STDERR_FILENO, "minishell: Unclosed quote detected.\n");
		return (ft_lstclear(&head, free_token), NULL);
	}
	return (head);
}

