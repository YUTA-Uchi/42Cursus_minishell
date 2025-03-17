/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/17 13:02:23 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_pwd(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	char	*absolute_path_buff;
	t_cmd	*cmd_content;

	(void)shell_state;
	(void)self;
	cmd_content = (t_cmd *)(current_cmd->content);
	if (cmd_content->args[1])
		return (print_const_error(PWD_TOO_MANY_ARGS, E_GENERAL_ERR));
	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (!absolute_path_buff)
		return (print_const_error(BUFF_NULL, E_GENERAL_ERR));
	if (!getcwd(absolute_path_buff, PATH_MAX))
	{
		free(absolute_path_buff);
		return (print_strerror("getcwd"), E_GENERAL_ERR);
	}
	ft_printf(STDOUT_FILENO, "%s\n", absolute_path_buff);
	free(absolute_path_buff);
	return (0);
}
