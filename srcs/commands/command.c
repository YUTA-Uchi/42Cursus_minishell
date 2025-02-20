/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:32:23 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/20 19:15:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "command.h"

t_list	*create_cmd(void)
{
	t_cmd		*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_name = NULL;
	cmd->args = ft_calloc(1, sizeof(char *));
	if (!cmd->args)
		return (free_cmd(cmd), NULL);
	cmd->redirections = ft_calloc(1, sizeof(t_list));
	if (!cmd->redirections)
		return (free_cmd(cmd), NULL);
	cmd->pid = 0;
	return (ft_lstnew(cmd));
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

void	free_cmd(void *cmd)
{
	t_cmd	*cmd_ptr;

	if (!cmd)
		return ;
	cmd_ptr = (t_cmd *)cmd;
	if (cmd_ptr->cmd_name)
		free(cmd_ptr->cmd_name);
	if (cmd_ptr->args)
		free_arr(cmd_ptr->args);
	if (cmd_ptr->redirections)
		free_redirection(&cmd_ptr->redirections);
	free(cmd_ptr);
}

void	free_cmd_list(t_list **cmds)
{
	ft_lstclear(cmds, free_cmd);
}
