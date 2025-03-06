/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:54:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/06 13:09:37 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

const t_builtins	*create_builtins_list(void)
{
	static const t_builtins	builtins_list[8] = {\
		{BUILTIN_ECHO, ft_echo}, \
		{BUILTIN_CD, ft_cd}, \
		{BUILTIN_PWD, ft_pwd}, \
		{BUILTIN_EXPORT, ft_export}, \
		{BUILTIN_UNSET, ft_unset}, \
		{BUILTIN_ENV, ft_env}, \
		{BUILTIN_EXIT, ft_exit}, \
		{NULL, NULL}
	};

	return (builtins_list);
}

const t_builtins	*lookup_builtin(char *cmd_name, \
									const t_builtins *builtins_list)
{
	const t_builtins	*ret;
	int					builtin_len;
	int					cmd_len;
	int					search_len;

	ret = builtins_list;
	cmd_len = ft_strlen(cmd_name);
	while (ret->name)
	{
		builtin_len = ft_strlen(ret->name);
		if (cmd_len < builtin_len)
			search_len = builtin_len;
		else
			search_len = cmd_len;
		if (ft_strncmp(ret->name, cmd_name, search_len) == 0)
			break ;
		ret++;
	}
	return (ret);
}
