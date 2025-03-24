/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:26:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 16:34:40 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLER_H
# define ERROR_HANDLER_H

# include "minishell.h"

# define ERR_HEADER "minishell: "
# define MALLOCF "malloc failed"
# define SYNERR "syntax error"
# define SYNERR_NEWLINE "syntax error near unexpected token `newline'"
# define SYNERR_UNEXPECTED "syntax error near unexpected token `%s'"
# define SYNERR_NEAR_PIPE "syntax error near unexpected token '|'"
# define SYNERR_NEAR_RIN "syntax error near unexpected token '<'"
# define SYNERR_NEAR_ROUT "syntax error near unexpected token '>'"
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
# define UNSET_TOO_MANY_ARGS "unset: too many arguments"
// builtin exit error message
# define EXIT_TOO_MANY_ARGS "exit: too many arguments"
# define EXIT_NUMERIC_ARG "exit: numeric argument required"

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
void			fatal_error(const char *function_name, const char *msg \
							, int status) __attribute__((noreturn));
int				get_err_status(void);
void			set_error(t_error_handler *self, int error, const char *msg);
void			print_strerror(char *funcname);
bool			print_error(const char *msg);
int				print_error_with_status(const char *msg, int status);
#endif