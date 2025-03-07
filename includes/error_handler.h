/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:26:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/07 14:22:27 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLER_H
# define ERROR_HANDLER_H

# include "minishell.h"

# define ERR_HEADER "minishell: "
# define COMMAND_NOT_FOUND "command not found"
// builtin pwd error message
# define BUFF_NULL "buff is null"
# define PWD_TOO_MANY_ARGS "pwd: too many arguments"
// buitin cd error message
# define CD_HOME_NOT_SET "cd: HOME not set"
# define CD_TOO_MANY_ARGS "cd: too many arguments"
//builtin env error message
# define ENV_TOO_MANY_ARGS "env: too many arguments"
# define ENV_NULL "environment variables are NULL"
// builtin unset error message

typedef enum e_error	t_error;

enum	e_error
{
	E_SUCCESS = 0,
	E_GENERAL_ERR = 1,
	E_NOPERMISSION = 126,
	E_NOTFOUND = 127,
	E_SIGTERM = 128
};

typedef struct s_error_handler
{
	t_error			error;
	const char		*msg;
}	t_error_handler;

t_error_handler	*create_error_handler(void);
void			free_error_handler(t_error_handler *error_handler);
void			fatal_error(const char *function_name, const char *msg) \
							__attribute__((noreturn));
int				get_err_status(void);
void			set_error(t_error_handler *self, int error, const char *msg);
void			print_strerror(char *funcname);
#endif