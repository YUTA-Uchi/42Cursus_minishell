/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:10:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/18 17:24:20 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	replace_env_variable_with_env_value(t_expand *expand_context \
											, char c, t_list *env_list)
{
	if (c == '\"')
	{
		if (!set_env_value_to_str(expand_context, env_list))
			return (false);
		expand_context->state = expand_context->prev_state;
	}
	else if (c == '\'')
	{
		if (!set_env_value_to_str(expand_context, env_list))
			return (false);
		if (expand_context->prev_state == EXPAND_IN_DOUBLE_QUOTE)
		{
			if (!append_char_to_str(expand_context, c))
				return (false);
		}
		expand_context->state = expand_context->prev_state;
	}
	else if (c == '$')
	{
		if (!set_env_value_to_str(expand_context, env_list))
			return (false);
	}
	return (true);
}

bool	expand_state_in_env(t_expand *expand_context \
							, char c, t_list *env_list)
{
	if (!replace_env_variable_with_env_value(expand_context, c, env_list))
		return (false);
	else if (c == '?')
	{
		if (!append_last_status_to_str(expand_context))
			return (false);
		expand_context->state = expand_context->prev_state;
	}
	else
	{
		if (!append_char_to_env_key(expand_context, c))
			return (false);
	}
	return (true);
}
