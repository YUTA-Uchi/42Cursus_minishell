/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:26:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 12:40:58 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLER_H
# define ERROR_HANDLER_H

# include "minishell.h"

# define ERR_HEADER "minishell: "
# define MALLOCF "malloc failed"
# define UNCLOSED_QUOTE "Unclosed quote detected."
# define SYNERR "syntax error"
# define SYNERR_NEWLINE "syntax error near unexpected token `newline'"
# define SYNERR_UNEXPECTED "%ssyntax error near unexpected token `%s'\n"
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
// builtin export error message
# define EXPORT_NO_VALID_IDENTIFIER "export: not a valid identifier"
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
	E_SYNTAX_ERR = 2,
	E_PIPE_ERR = 3,
	E_REDIRECTION_ERR = 4,
	E_NOPERMISSION = 126,
	E_NOTFOUND = 127,
	E_SIGTERM = 128
};

void			fatal_error(const char *function_name, const char *msg \
							, int status) __attribute__((noreturn));
int				get_err_status(void);
void			print_strerror(const char *funcname);
bool			print_error(const char *msg);
int				print_error_with_status(const char *msg, int status);
bool			print_error_with_value(const char *err_msg \
									, const char *variable_value);
#endif