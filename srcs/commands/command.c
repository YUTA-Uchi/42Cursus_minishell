/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:32:23 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 13:28:09 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "command.h"

t_cmd	*create_cmd(char *line)
{
	t_cmd		*cmd;
	char		*args[] = {line, NULL};

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit(1);
	cmd->cmd_name = strdup(line);
	if (!cmd->cmd_name)
		exit(1);
	cmd->args = args;
	cmd->pid = 0;
	return (cmd);
}

void	free_arr(char **data)
{
	char	**data_origin;

	data_origin = data;
	while (*data)
	{
		free(*data);
		data++;
	}
	free(data_origin);
}

void	free_cmd(t_cmd *cmd)
{
	free(cmd->cmd_name);
	free_arr(cmd->args);
	free(cmd->redirections);
	free(cmd);
}

void	free_cmd_list(t_list *cmds)
{
	t_list	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free_cmd(cmds->content);
		free(cmds);
		cmds = tmp;
	}
}
