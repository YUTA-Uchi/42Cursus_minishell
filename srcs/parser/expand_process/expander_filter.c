/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_filter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:59:34 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 18:21:19 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander_filter.h"

bool	apply_expansion_filters(t_list **tokens, t_shell_state *state)
{
	static const t_expand_filter	filters[] = {\
		{"variables", expand_variables, true}, \
		{"empty", remove_empty_tokens, false}, \
		{NULL, NULL, false}
	};
	int								i;

	i = 0;
	while (filters[i].name != NULL)
	{
		if (!filters[i].filter(tokens, state) && filters[i].required)
			return (false);
		i++;
	}
	return (true);
}