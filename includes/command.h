/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:26:54 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 16:17:56 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "minishell.h"
# include "redirection.h"

typedef struct s_cmd	t_cmd;

struct s_cmd
{
	pid_t			pid;
	char			*cmd_name;
	char			**args;
	t_redirection	*redirections;
	t_cmd			*next;
};

t_cmd	*create_cmd(char *line);
void	free_cmd(t_cmd *cmd);
void	free_arr(char **data);
#endif