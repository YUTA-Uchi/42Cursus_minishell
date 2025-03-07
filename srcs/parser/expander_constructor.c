/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:18:46 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 11:20:46 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

t_expand	*create_expand(t_shell_state *shell_state)
{
	t_expand	*expand;

	expand = malloc(sizeof(t_expand));
	if (!expand)
		return (NULL);
	expand->state = EXPAND_WORD;
	expand->prev_state = EXPAND_WORD;
	expand->shell_state = shell_state;
	expand->value = malloc(sizeof(char) * 1);
	if (!expand->value)
		return (free(expand), NULL);
	expand->value[0] = '\0';
	expand->len = 0;
	expand->capacity = 1;
	expand->env_key = malloc(sizeof(char) * 1);
	if (!expand->env_key)
		return (free(expand->value), free(expand), NULL);
	expand->env_key[0] = '\0';
	expand->env_key_len = 0;
	expand->env_key_capacity = 1;
	return (expand);
}

void	free_expand(t_expand *expand)
{
	if (expand->value)
		free(expand->value);
	if (expand->env_key)
		free(expand->env_key);
	free(expand);
}
