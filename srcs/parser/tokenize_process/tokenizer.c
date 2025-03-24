/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:22:57 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 19:48:28 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_list	*create_token(t_token_type type, const char value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (!value)
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

void	free_token(void *token)
{
	t_token	*token_content;

	if (!token)
		return ;
	token_content = (t_token *)token;
	if (token_content->value)
		free(token_content->value);
	free(token_content);
}

t_list	*tokenize_line(const char *line)
{
	t_list	*token_list;
	t_state	state;
	int		i;

	token_list = NULL;
	state = STATE_NONE;
	i = 0;
	while (line[i])
	{
		if (!handle_tokenize_state(&state, &token_list, line[i]))
			return (ft_lstclear(&token_list, free_token), NULL);
		i++;
	}
	if (state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE)
	{
		ft_printf(STDERR_FILENO, "minishell: Unclosed quote detected.\n");
		return (ft_lstclear(&token_list, free_token), NULL);
	}
	return (token_list);
}
