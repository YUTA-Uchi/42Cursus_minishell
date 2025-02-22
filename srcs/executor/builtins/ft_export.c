/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/22 19:01:59 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"
#include "builtin.h"

int	ft_setenv(char *key, char *value, int overwrite)
{
	if (setenv(key, value, overwrite) == -1) // setenv could not be used
		return (fatal_error("ft_export: ", strerror(errno)), get_err_status());
	return (0);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*new_str;
	size_t	i;

	new_str = (char *)malloc(n + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	ft_export(t_cmd *cmd, t_error_handler *error_handler, t_list *env_list)
{
	int		i;
	char	*key;
	char	*value;

	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '='))
		{
			key = ft_strndup(cmd->args[i], ft_strchr(cmd->args[i], '=') - cmd->args[i]);
			value = ft_strdup(ft_strchr(cmd->args[i], '=') + 1);
			if (!add_env(env_list, key, value))
				return (set_error(error_handler, E_GENERAL_ERR, strerror(errno)), 1);
		}
		else
		{
			if (ft_setenv(cmd->args[i], "", 1) == -1)
				return (set_error(error_handler, E_GENERAL_ERR, strerror(errno)), 1);
		}
		i++;
	}
	return (0);
}