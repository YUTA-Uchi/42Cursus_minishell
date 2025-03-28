/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:58:30 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 17:20:02 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_STATE_H
# define SHELL_STATE_H

# include "minishell.h"
# include "environment.h"
# include "syscall_wrapper.h"

typedef struct s_shell_state
{
	bool			running;
	int				last_status;
	bool			is_interactive;
	t_list			*env_list;
}	t_shell_state;

t_shell_state	*create_shell_state(char **environ);
void			free_shell_state(t_shell_state *shell_state);
#endif