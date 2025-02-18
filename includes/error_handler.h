/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:26:59 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 17:10:05 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLER_H
# define ERROR_HANDLER_H

# define ERR_HEADER "minishell: "
# define COMMAND_NOT_FOUND "command not found"

typedef enum e_error	t_error;

typedef struct s_error_handler
{
	t_error		error;
	char		*msg;
}	t_error_handler;

enum	e_error
{
	E_SUCCESS = 0,
	E_GENERAL_ERR = 1,
	E_NOPERMISSION = 126,
	E_NOTFOUND = 127,
	E_SIGTERM = 128
};

t_error_handler	*create_error_handler(void);
void			free_error_handler(t_error_handler *error_handler);
void			fatal_error(const char *function_name, const char *msg) \
							__attribute__((noreturn));
#endif