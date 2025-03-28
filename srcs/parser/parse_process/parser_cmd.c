/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:42:51 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/28 12:30:40 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	add_arg(char ***args, char *arg)
{
	char	**new_args;
	int		i;

	if (!args || !arg)
		return (false);
	i = 0;
	while ((*args)[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (free(arg), false);
	i = 0;
	while ((*args)[i])
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[i] = arg;
	new_args[i + 1] = NULL;
	free(*args);
	*args = new_args;
	return (true);
}

bool	parse_command_args(t_token *token_content, t_cmd *cmd_content)
{
	char	*new_arg;

	if (token_content->type != TOKEN_WORD)
		return (true);
	if (!cmd_content->cmd_name)
	{
		cmd_content->cmd_name = ft_strdup(token_content->value);
		if (cmd_content->cmd_name == NULL)
			return (print_error(MALLOCF));
	}
	new_arg = ft_strdup(token_content->value);
	if (!new_arg)
		return (print_error(MALLOCF));
	return (add_arg(&(cmd_content->args), new_arg));
}
