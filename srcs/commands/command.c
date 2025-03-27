/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:32:23 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/27 18:12:18 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	cmd->redirections = NULL;
	cmd->pid = -1;
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
	t_cmd	*cmd_content;

	if (!cmd)
		return ;
	cmd_content = (t_cmd *)cmd;
	if (cmd_content->cmd_name)
		free(cmd_content->cmd_name);
	if (cmd_content->args)
		free_arr(cmd_content->args);
	if (cmd_content->redirections)
		ft_lstclear(&cmd_content->redirections, free_redirection);
	free(cmd_content);
}

void	free_cmd_list(t_list **cmd_list)
{
	ft_lstclear(cmd_list, free_cmd);
}
