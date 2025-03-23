/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_string_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:20:07 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/23 13:25:46 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

bool	append_char_to_env_key(t_expand *expand_ctx, char c)
{
	char	*new_key;

	if (expand_ctx->env_key_len + 1 >= expand_ctx->env_key_capacity)
	{
		expand_ctx->env_key_capacity *= 2;
		new_key = malloc(sizeof(char) * (expand_ctx->env_key_capacity));
		if (!new_key)
			return (false);
		ft_strlcpy(new_key, expand_ctx->env_key, expand_ctx->env_key_len + 1);
		free(expand_ctx->env_key);
		expand_ctx->env_key = new_key;
	}
	expand_ctx->env_key[expand_ctx->env_key_len++] = c;
	expand_ctx->env_key[expand_ctx->env_key_len] = '\0';
	return (true);
}

bool	append_char_to_str(t_expand *expand_ctx, char c)
{
	char	*new_value;

	if (expand_ctx->len + 1 >= expand_ctx->capacity)
	{
		expand_ctx->capacity *= 2;
		new_value = malloc(sizeof(char) * (expand_ctx->capacity));
		if (!new_value)
			return (false);
		ft_strlcpy(new_value, expand_ctx->value, expand_ctx->len + 1);
		free(expand_ctx->value);
		expand_ctx->value = new_value;
	}
	expand_ctx->value[expand_ctx->len++] = c;
	expand_ctx->value[expand_ctx->len] = '\0';
	return (true);
}

bool	append_last_status_to_str(t_expand *expand_ctx)
{
	char	*status_str;
	int		i;

	status_str = ft_itoa(expand_ctx->shell_state->last_status);
	if (!status_str)
		return (false);
	i = 0;
	while (status_str[i])
	{
		if (!append_char_to_str(expand_ctx, status_str[i]))
		{
			free(status_str);
			return (false);
		}
		i++;
	}
	free(status_str);
	return (true);
}
