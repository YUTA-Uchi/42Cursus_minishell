/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:54:29 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 16:58:09 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

const t_builtins	*create_builtins_list(void)
{
	static const t_builtins	builtins_list[] = {\
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
	int					i;
	const t_builtins	*ret;

	i = 0;
	ret = builtins_list;
	while (ret->name)
	{
		if (strcmp(ret->name, cmd_name) == 0)
			break ;
		i++;
		ret++;
	}
	return (ret);
}