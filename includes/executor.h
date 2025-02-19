/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 16:40:23 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"

# define BUILTIN_ECHO	"echo"
# define BUILTIN_CD		"cd"
# define BUILTIN_PWD	"pwd"
# define BUILTIN_EXPORT	"export"
# define BUILTIN_UNSET	"unset"
# define BUILTIN_ENV	"env"
# define BUILTIN_EXIT	"exit"

typedef struct s_builtins
{
	char	*name;
	int		(*func)(t_cmd *, t_error_handler *);
}	t_builtins;

typedef struct s_executor	t_executor;
struct s_executor
{
	t_cmd				*cmds;
	t_error_handler		*error_handler;
	const t_builtins	*builtins_list;
	int					(*excute)(t_executor *);
};

t_executor	*create_executor(void);
void		free_executor(t_executor *executor);

// builtins
int			ft_echo(t_cmd *cmd, t_error_handler *error_handler);
int			ft_cd(t_cmd *cmd, t_error_handler *error_handler);
int			ft_pwd(t_cmd *cmd, t_error_handler *error_handler);
int			ft_export(t_cmd *cmd, t_error_handler *error_handler);
int			ft_unset(t_cmd *cmd, t_error_handler *error_handler);
int			ft_env(t_cmd *cmd, t_error_handler *error_handler);
int			ft_exit(t_cmd *cmd, t_error_handler *error_handler);
#endif