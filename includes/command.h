/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:26:54 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/26 11:17:04 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "minishell.h"
# include "redirection.h"

typedef struct s_cmd	t_cmd;

struct s_cmd
{
	pid_t	pid;
	char	*cmd_name;
	char	**args;
	t_list	*redirections;
};

t_list	*create_cmd(void);
void	free_cmd(void *cmd);
void	free_arr(char **data);
void	free_cmd_list(t_list **cmds);

#endif