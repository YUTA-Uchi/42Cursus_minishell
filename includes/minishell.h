/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:35:35 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/17 18:10:47 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"


# define PROMPT "minishell$ "
# define ERR_HEADER "minishell: "
# define COMMAND_NOT_FOUND "command not found"

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
	pid_t	pid;
}	t_cmd;

enum	e_error
{
	E_SUCCESS = 0,
	E_GENERAL_ERR = 1,
	E_NOPERMISSION = 126,
	E_NOTFOUND = 127,
	E_SIGTERM = 128
};

#endif