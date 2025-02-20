/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_manager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:04 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 12:36:17 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_MANAGER_H
# define SHELL_MANAGER_H


# include "minishell.h"
# include "parser.h"
# include "executor.h"
# include "error_handler.h"

# define PROMPT "minishell$ "

typedef struct s_shell_manager	t_shell_manager;

struct s_shell_manager
{
	t_parser		*parser;
	t_executor		*executor;
	t_error_handler	*error_handler;
	char			*(*listen)(t_shell_manager *, const char *);
};

void	set_line(t_shell_manager *shell_manager);
void	set_cmds(t_shell_manager *shell_manager);
t_shell_manager	*create_shell_manager(void);
void			free_shell_manager(t_shell_manager *shell_manager);

#endif
