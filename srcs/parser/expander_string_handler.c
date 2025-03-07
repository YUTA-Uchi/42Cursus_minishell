/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_string_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:20:07 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 12:08:12 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

bool	append_char_to_env_key(t_expand *expand, char c)
{
	char	*new_key;

	if (expand->env_key_len + 1 >= expand->env_key_capacity)
	{
		expand->env_key_capacity *= 2;
		new_key = malloc(sizeof(char) * (expand->env_key_capacity));
		if (!new_key)
			return (false);
		ft_strlcpy(new_key, expand->env_key, expand->env_key_len + 1);
		free(expand->env_key);
		expand->env_key = new_key;
	}
	expand->env_key[expand->env_key_len++] = c;
	expand->env_key[expand->env_key_len] = '\0';
	return (true);
}

bool	append_char_to_str(t_expand *expand, char c)
{
	char	*new_value;

	if (expand->len + 1 >= expand->capacity)
	{
		expand->capacity *= 2;
		new_value = malloc(sizeof(char) * (expand->capacity));
		if (!new_value)
			return (false);
		ft_strlcpy(new_value, expand->value, expand->len + 1);
		free(expand->value);
		expand->value = new_value;
	}
	expand->value[expand->len++] = c;
	expand->value[expand->len] = '\0';
	return (true);
}

bool	append_last_status_to_str(t_expand *expand)
{
	char	*status_str;
	int		i;

	status_str = ft_itoa(expand->shell_state->last_status);
	if (!status_str)
		return (false);
	i = 0;
	while (status_str[i])
	{
		if (!append_char_to_str(expand, status_str[i]))
		{
			free(status_str);
			return (false);
		}
		i++;
	}
	free(status_str);
	return (true);
}

bool	set_env_value_to_str(t_expand *expand_context, t_list *env_list)
{
	char	*env_value;
	int		i;

	env_value = get_env_value(env_list, expand_context->env_key);
	if (env_value)
	{
		i = 0;
		while (env_value[i])
		{
			if (!append_char_to_str(expand_context, env_value[i++]))
				return (false);
		}
	}
	free(expand_context->env_key);
	expand_context->env_key = malloc(sizeof(char) * 1);
	if (!expand_context->env_key)
		return (false);
	expand_context->env_key_len = 0;
	expand_context->env_key_capacity = 1;
	return (true);
}
