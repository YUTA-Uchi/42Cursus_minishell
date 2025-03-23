/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:18:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/23 13:07:13 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

t_expand	*create_expand(t_shell_state *shell_state)
{
	t_expand	*expand_ctx;

	expand_ctx = malloc(sizeof(t_expand));
	if (!expand_ctx)
		return (NULL);
	expand_ctx->state = EXPAND_WORD;
	expand_ctx->prev_state = EXPAND_WORD;
	expand_ctx->shell_state = shell_state;
	expand_ctx->value = malloc(sizeof(char) * 1);
	if (!expand_ctx->value)
		return (free_expand(expand_ctx), NULL);
	expand_ctx->value[0] = '\0';
	expand_ctx->len = 0;
	expand_ctx->capacity = 1;
	expand_ctx->env_key = malloc(sizeof(char) * 1);
	if (!expand_ctx->env_key)
		return (free_expand(expand_ctx), NULL);
	expand_ctx->env_key[0] = '\0';
	expand_ctx->env_key_len = 0;
	expand_ctx->env_key_capacity = 1;
	return (expand_ctx);
}

bool	initialize_env_key(t_expand *expand_ctx)
{
	free(expand_ctx->env_key);
	expand_ctx->env_key = malloc(sizeof(char) * 1);
	if (!expand_ctx->env_key)
		return (false);
	expand_ctx->env_key_len = 0;
	expand_ctx->env_key_capacity = 1;
	return (true);
}

void	free_expand(t_expand *expand_ctx)
{
	if (expand_ctx->value)
		free(expand_ctx->value);
	if (expand_ctx->env_key)
		free(expand_ctx->env_key);
	free(expand_ctx);
}
