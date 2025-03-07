/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_state_handler.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:13:13 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 11:38:25 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	expand_state_in_env(t_expand *expand_context \
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
	else if (c == '?')
	{
		if (!append_last_status_to_str(expand_context))
			return (false);
		expand_context->state = expand_context->prev_state;
	}
	else if (c == '$')
	{
		if (!set_env_value_to_str(expand_context, env_list))
			return (false);
	}
	else
	{
		if (!append_char_to_env_key(expand_context, c))
			return (false);
	}
	return (true);
}

static bool	expand_state_in_double_quote(t_expand *expand_context \
										, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\"')
	{
		expand_context->state = EXPAND_WORD;
		expand_context->prev_state = EXPAND_WORD;
	}
	else if (c == '$')
	{
		expand_context->state = EXPAND_IN_ENV;
		expand_context->prev_state = EXPAND_IN_DOUBLE_QUOTE;
	}
	else
	{
		if (!append_char_to_str(expand_context, c))
			return (false);
	}
	return (true);
}

static bool	expand_state_in_single_quote(t_expand *expand_context \
										, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\'')
		expand_context->state = EXPAND_WORD;
	else
	{
		if (!append_char_to_str(expand_context, c))
			return (false);
	}
	return (true);
}

static bool	expand_state_word(t_expand *expand_context \
							, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\'')
		expand_context->state = EXPAND_IN_SINGLE_QUOTE;
	else if (c == '\"')
		expand_context->state = EXPAND_IN_DOUBLE_QUOTE;
	else if (c == '$')
		expand_context->state = EXPAND_IN_ENV;
	else
	{
		if (!append_char_to_str(expand_context, c))
			return (false);
	}
	return (true);
}

bool	handle_expand_state(t_expand *expand, char c, t_list *env_list)
{
	const t_expand_state_handler	expand_state_handler[4] = {\
		expand_state_word, \
		expand_state_in_single_quote, \
		expand_state_in_double_quote, \
		expand_state_in_env
	};

	return (expand_state_handler[expand->state](expand, c, env_list));
}
