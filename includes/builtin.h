/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:56:27 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 17:03:18 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"
# include "environment.h"
# include "executor.h"

# define BUILTIN_ECHO	"echo"
# define BUILTIN_CD		"cd"
# define BUILTIN_PWD	"pwd"
# define BUILTIN_EXPORT	"export"
# define BUILTIN_UNSET	"unset"
# define BUILTIN_ENV	"env"
# define BUILTIN_EXIT	"exit"

const t_builtins	*create_builtins_list(void);
const t_builtins	*lookup_builtin(char *cmd_name, \
									const t_builtins *builtins_list);

// builtins
int					ft_echo(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);
int					ft_cd(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);
int					ft_pwd(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);
int					ft_export(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);
int					ft_unset(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);
int					ft_env(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);
int					ft_exit(t_executor *self, t_list *current_cmd \
							, t_shell_state *shell_state);

#endif