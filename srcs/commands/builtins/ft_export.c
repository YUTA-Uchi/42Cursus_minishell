/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/19 14:45:02 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	ft_setenv(char *key, char *value, int overwrite)
{
	if (setenv(key, value, overwrite) == -1) // setenv could not be used
		return (fatal_error("ft_export: ", strerror(errno)), get_err_status());
	return (0);
}

int	ft_export(t_cmd *cmd, t_error_handler *error_handler)
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
			if (key == NULL || value == NULL)
				return (fatal_error("ft_export: ", strerror(errno)), get_err_status());
			if (ft_setenv(key, value, 1) == -1)
				return (fatal_error("ft_export: ", strerror(errno)), get_err_status());
			free(key);
			free(value);
		}
		else
		{
			if (ft_setenv(cmd->args[i], "", 1) == -1)
				return (fatal_error("ft_export: ", strerror(errno)), get_err_status());
		}
		i++;
	}
	return (0);
}