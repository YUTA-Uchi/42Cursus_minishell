/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:10:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/30 15:31:01 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	append_env_value(t_expand *expand_ctx, char *env_value)
{
	int	i;

	if (!env_value)
		return (true);
	i = 0;
	while (env_value[i])
	{
		if (!append_char_to_str(expand_ctx, env_value[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	set_env_value_to_str(t_expand *expand_ctx, t_list *env_list)
{
	char	*env_value;

	if (expand_ctx->env_key_len == 0)
	{
		return (append_char_to_str(expand_ctx, '$'));
	}
	env_value = get_env_value(env_list, expand_ctx->env_key);
	if (!append_env_value(expand_ctx, env_value))
		return (false);
	return (initialize_env_key(expand_ctx));
}

static bool	handle_quote_in_env(t_expand *expand_ctx, char c \
								, t_list *env_list)
{
	if (!set_env_value_to_str(expand_ctx, env_list))
		return (false);
	if (c == '\"' || (c == '\'' && \
			expand_ctx->prev_state != EXPAND_IN_DOUBLE_QUOTE))
	{
		expand_ctx->state = expand_ctx->prev_state;
		return (true);
	}
	if (c == '\'' && expand_ctx->prev_state == EXPAND_IN_DOUBLE_QUOTE)
	{
		if (!append_char_to_str(expand_ctx, c))
			return (false);
		expand_ctx->state = expand_ctx->prev_state;
	}
	return (true);
}

static bool	replace_env_variable_with_env_value(t_expand *expand_ctx \
												, char c, t_list *env_list)
{
	if (c == '\"' || c == '\'')
		return (handle_quote_in_env(expand_ctx, c, env_list));
	if (c == '$')
	{
		if (!set_env_value_to_str(expand_ctx, env_list))
			return (false);
	}
	if (ft_is_space(c))
	{
		if (!set_env_value_to_str(expand_ctx, env_list))
			return (false);
		if (!append_char_to_str(expand_ctx, c))
			return (false);
		expand_ctx->state = expand_ctx->prev_state;
	}
	return (true);
}

bool	expand_state_in_env(t_expand *expand_ctx \
							, char c, t_list *env_list)
{
	if (!replace_env_variable_with_env_value(expand_ctx, c, env_list))
		return (false);
	if (c == '?')
	{
		if (!append_last_status_to_str(expand_ctx))
			return (false);
		expand_ctx->state = expand_ctx->prev_state;
		return (true);
	}
	if (!ft_is_space(c))
	{
		if (!append_char_to_env_key(expand_ctx, c))
			return (false);
		return (true);
	}
	return (true);
}
