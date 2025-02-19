/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:22:57 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 19:50:52 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token **tail, t_token *token)
{
	if (!*head)
	{
		*head = token;
		*tail = token;
	}
	else
	{
		(*tail)->next = token;
		*tail = token;
	}
}

#define INITIAL_TOKEN_BUF_SIZE 64

char	*append_char(char *buffer, int *length, int *capacity, char c)
{
	int		new_capacity;
	char	*new_buffer;
	int		i;

	if (*length + 1 >= *capacity)
	{
		new_capacity = (*capacity) * 2;
		new_buffer = malloc(new_capacity);
		if (!new_buffer)
			return (NULL);
		i = 0;
		while (i < *length)
		{
			new_buffer[i] = buffer[i];
			i++;
		}
		free(buffer);
		buffer = new_buffer;
		*capacity = new_capacity;
	}
	buffer[*length] = c;
	(*length)++;
	buffer[*length] = '\0';
	return (buffer);
}

bool	tokenize_state_none(t_state *state, t_token **head, t_token **tail, char **token_buf, int *token_len, int *token_capacity, char c)
{
	if (isspace(c))
	{
		/* スペースは無視 */
	}
	else if (c == '<')
	{
		t_token *token = create_token(TOKEN_REDIR_IN, "<");
		append_token(head, tail, token);
	}
	else if (c == '>')
	{
		t_token *token = create_token(TOKEN_REDIR_OUT, ">");
		append_token(head, tail, token);
	}
	else if (c == '|')
	{
		t_token *token = create_token(TOKEN_PIPE, "|");
		append_token(head, tail, token);
	}
	else if (c == '\'')
	{
		*state = STATE_IN_SINGLE_QUOTE;
	}
	else if (c == '\"')
	{
		*state = STATE_IN_DOUBLE_QUOTE;
	}
	else
	{
		*state = STATE_WORD;
		*token_buf = append_char(*token_buf, token_len, token_capacity, c);
		if (!*token_buf)
			return (false);
	}
	return (true);
}

bool	tokenize_state_word(t_state *state, t_token **head, t_token **tail, char **token_buf, int *token_len, int *token_capacity, char c)
{
	if (isspace(c))
	{
		/* WORDトークンの終了 */
		t_token *token = create_token(TOKEN_WORD, token_buf);
		append_token(&head, &tail, token);
		state = STATE_NONE;
		token_len = 0;
		token_buf[0] = '\0';
	}
	else if (c == '<' || c == '>' || c == '|')
	{
		/* 現在のWORDを終了し、特殊文字を別トークンとして追加 */
		t_token *token = create_token(TOKEN_WORD, token_buf);
		append_token(&head, &tail, token);
		token_len = 0;
		token_buf[0] = '\0';
		if (c == '<')
		{
			token = create_token(TOKEN_REDIR_IN, "<");
			append_token(&head, &tail, token);
		}
		else if (c == '>')
		{
			token = create_token(TOKEN_REDIR_OUT, ">");
			append_token(&head, &tail, token);
		}
		else if (c == '|')
		{
			token = create_token(TOKEN_PIPE, "|");
			append_token(&head, &tail, token);
		}
		state = STATE_NONE;
	}
	else if (c == '\'')
	{
		/* WORDの途中でシングルクォート開始 */
		state = STATE_IN_SINGLE_QUOTE;
	}
	else if (c == '\"')
	{
		/* WORDの途中でダブルクォート開始 */
		state = STATE_IN_DOUBLE_QUOTE;
	}
	else
	{
		token_buf = append_char(token_buf, &token_len, &token_capacity, c);
		if (!token_buf)
			return (false);
	}
	return (true);
}

bool	tokenize_state_in_single_quote(t_state *state, t_token **head, t_token **tail, char **token_buf, int *token_len, int *token_capacity, char c)
{
	if (c == '\'')
	{
		/* シングルクォート終了 → WORDに戻る */
		state = STATE_WORD;
	}
	else
	{
		token_buf = append_char(token_buf, &token_len, &token_capacity, c);
		if (!token_buf)
			return (false);
	}
	return (true);
}

bool	tokenize_state_in_double_quote(t_state *state, t_token **head, t_token **tail, char **token_buf, int *token_len, int *token_capacity, char c)
{
	if (c == '\"')
	{
		/* ダブルクォート終了 → WORDに戻る */
		state = STATE_WORD;
	}
	else
	{
		token_buf = append_char(token_buf, &token_len, &token_capacity, c);
		if (!token_buf)
			return (false);
	}
	return (true);
}

t_token	*tokenize_line(const char *line)
{
	t_token	*head;
	t_token	*tail;
	t_state	state;
	int		token_len;
	int		token_capacity;
	char	*token_buf;
	int		i;

	head = NULL;
	tail = NULL;
	state = STATE_NONE;
	token_len = 0;
	token_capacity = INITIAL_TOKEN_BUF_SIZE;
	token_buf = malloc(token_capacity);
	if (!token_buf)
		return (NULL);
	token_buf[0] = '\0';
	i = 0;
	while (line[i])
	{
		char c = line[i];
		if (state == STATE_NONE)
		{
			if (!tokenize_state_none(&state, &head, &tail, &token_buf, &token_len, &token_capacity, c))
				return (NULL);
		}
		else if (STATE_WORD)
		{
			if (!tokenize_state_word(&state, &head, &tail, &token_buf, &token_len, &token_capacity, c))
				return (NULL);
		}
		else if (state == STATE_IN_SINGLE_QUOTE)
		{
			if (!tokenize_state_in_single_quote(&state, &head, &tail, &token_buf, &token_len, &token_capacity, c))
				return (NULL);
		}
		else if (state == STATE_IN_DOUBLE_QUOTE)
		{
			if (!tokenize_state_in_double_quote(&state, &head, &tail, &token_buf, &token_len, &token_capacity, c))
				return (NULL);
		}
			i++;
	}
	if (state == STATE_WORD)
	{
		t_token *token = create_token(TOKEN_WORD, token_buf);
		append_token(&head, &tail, token);
	}
	else if (state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE)
	{
		fprintf(stderr, "Error: Unclosed quote detected.\n");
		free(token_buf);
		// ここでトークンリストも適切に解放するべき
		return (NULL);
	}
	free(token_buf);
	return (head);
}

