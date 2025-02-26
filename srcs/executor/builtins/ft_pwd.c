/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:32:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/22 19:53:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	ft_pwd(t_cmd *self, t_error_handler *error_handler, t_list *env_list)
{
	char	*absolute_path_buff;

	(void)env_list;
	if (self->args[1] != NULL)
	{
		set_error(error_handler, E_GENERAL_ERR, PWD_TOO_MANY_ARGS);
		return (1);
	}
	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (getcwd(absolute_path_buff, PATH_MAX) == NULL)
	{
		set_error(error_handler, E_GENERAL_ERR, strerror(errno));
		return (1);
	}
	if (!absolute_path_buff)
	{
		set_error(error_handler, E_GENERAL_ERR, BUFF_NULL);
		return (1);
	}
	ft_printf(STDOUT_FILENO, "%s\n", absolute_path_buff);
	free(absolute_path_buff);
	return (0);
}
