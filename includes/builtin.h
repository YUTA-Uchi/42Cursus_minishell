/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:56:27 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/27 17:47:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

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
int					ft_echo(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);
int					ft_cd(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);
int					ft_pwd(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);
int					ft_export(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);
int					ft_unset(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);
int					ft_env(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);
int					ft_exit(t_executor *self, t_error_handler *error_handler \
							, t_list *env_list);

#endif