/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_filter.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:57:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 17:57:48 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_FILTER_H
# define EXPANDER_FILTER_H

# include "parser.h"
# include "shell_state.h"
# include "error_handler.h"

typedef bool	(*t_token_filter)(t_list **tokens, t_shell_state *state);

typedef struct s_expand_filter
{
	const char		*name;
	t_token_filter	filter;
	bool			required;
}	t_expand_filter;

// フィルタチェーン管理関数
bool			apply_expansion_filters(t_list **tokens, t_shell_state *state);

// 各フィルタ関数
bool			expand_variables(t_list **tokens, t_shell_state *state);
bool			remove_quotes(t_list **tokens, t_shell_state *state);
bool			remove_empty_tokens(t_list **tokens, t_shell_state *state);

#endif
