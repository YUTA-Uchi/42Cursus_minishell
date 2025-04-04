/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:35:08 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/30 19:48:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static bool	is_valid_identifier(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (false);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static bool	process_key_value_pair(t_shell_state *shell_state \
									, char *arg, int *i)
{
	char	*key;
	char	*value;

	key = ft_strndup(arg, ft_strchr(arg, '=') - arg);
	if (!ft_strlen(key) || !is_valid_identifier(key))
	{
		free(key);
		print_error(EXPORT_NO_VALID_IDENTIFIER);
		(*i)++;
		return (shell_state->last_status = E_GENERAL_ERR, false);
	}
	value = ft_strdup(ft_strchr(arg, '=') + 1);
	if (!add_env(shell_state->env_list, key, value))
	{
		free(key);
		free(value);
		print_strerror("export");
		(*i)++;
		return (shell_state->last_status = E_GENERAL_ERR, false);
	}
	free(key);
	free(value);
	return (true);
}

static bool	process_key_only(t_shell_state *shell_state, char *arg, int *i)
{
	if (!ft_strlen(arg) || !is_valid_identifier(arg))
	{
		print_error(EXPORT_NO_VALID_IDENTIFIER);
		(*i)++;
		return (shell_state->last_status = E_GENERAL_ERR, false);
	}
	if (!get_env_value(shell_state->env_list, arg))
	{
		if (!add_env(shell_state->env_list, arg, NULL))
		{
			print_strerror("export");
			(*i)++;
			return (shell_state->last_status = EXIT_FAILURE, false);
		}
	}
	return (true);
}

int	ft_export(t_executor *self, t_list *current_cmd, t_shell_state *shell_state)
{
	int		i;
	t_cmd	*cmd_content;

	(void)self;
	i = 1;
	cmd_content = (t_cmd *)(current_cmd->content);
	if (!cmd_content->args[i])
		return (print_env(shell_state->env_list, true), 0);
	while (cmd_content->args[i])
	{
		if (ft_strchr(cmd_content->args[i], '='))
		{
			if (!process_key_value_pair(shell_state, cmd_content->args[i], &i))
				continue ;
		}
		else
		{
			if (!process_key_only(shell_state, cmd_content->args[i], &i))
				continue ;
		}
		i++;
	}
	return (shell_state->last_status);
}
