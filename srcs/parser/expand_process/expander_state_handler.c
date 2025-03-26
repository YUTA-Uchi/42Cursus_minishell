/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_state_handler.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:13:13 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/26 15:25:03 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	expand_state_in_double_quote(t_expand *expand_ctx \
										, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\"')
	{
		expand_ctx->state = EXPAND_WORD;
		expand_ctx->prev_state = EXPAND_WORD;
	}
	else if (c == '$')
	{
		expand_ctx->state = EXPAND_IN_ENV;
		expand_ctx->prev_state = EXPAND_IN_DOUBLE_QUOTE;
	}
	else
	{
		if (!append_char_to_str(expand_ctx, c))
			return (false);
	}
	return (true);
}

static bool	expand_state_in_single_quote(t_expand *expand_ctx \
										, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\'')
		expand_ctx->state = EXPAND_WORD;
	else
	{
		if (!append_char_to_str(expand_ctx, c))
			return (false);
	}
	return (true);
}

static bool	expand_state_word(t_expand *expand_ctx \
							, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\'')
		expand_ctx->state = EXPAND_IN_SINGLE_QUOTE;
	else if (c == '\"')
		expand_ctx->state = EXPAND_IN_DOUBLE_QUOTE;
	else if (c == '$')
		expand_ctx->state = EXPAND_IN_ENV;
	else
	{
		if (!append_char_to_str(expand_ctx, c))
			return (false);
	}
	return (true);
}

bool	dispatch_expand_state_handler(t_expand *expand_ctx \
									, char c, t_list *env_list)
{
	static const t_expand_state_handler	expand_state_handler[4] = {\
		expand_state_word, \
		expand_state_in_single_quote, \
		expand_state_in_double_quote, \
		expand_state_in_env
	};

	return (expand_state_handler[expand_ctx->state](expand_ctx, c, env_list));
}
