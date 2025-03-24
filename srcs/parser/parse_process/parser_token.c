/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:04:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/24 16:34:00 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static bool	add_cmd_node_to_list(t_list **cmd_list, t_list **current_cmd)
{
	if (!(*current_cmd))
	{
		*current_cmd = create_cmd();
		if (!(*current_cmd))
			return (print_error_with_status(MALLOCF, 0), NULL);
		ft_lstadd_back(cmd_list, *current_cmd);
	}
	return (true);
}

static bool	handle_pipe_token(t_list **current_cmd)
{
	if (!*current_cmd)
	{
		print_error_with_status(SYNERR_NEAR_PIPE, 0);
		return (false);
	}
	*current_cmd = NULL;
	return (true);
}

static bool	process_token(t_list **token_list \
					, t_list **current_cmd, t_list **cmd_list)
{
	t_token	*token_content;
	t_cmd	*cmd_content;

	token_content = (t_token *)((*token_list)->content);
	if (token_content->type == TOKEN_PIPE)
		return (handle_pipe_token(current_cmd));
	if (!add_cmd_node_to_list(cmd_list, current_cmd))
		return (false);
	cmd_content = (t_cmd *)((*current_cmd)->content);
	if (!parse_redirection(token_list, cmd_content))
		return (false);
	return (parse_command_args(token_content, cmd_content));
}

t_list	*parse_tokens(t_list *token_list, t_shell_state *shell_state)
{
	t_list	*cmd_list;
	t_list	*current_cmd;

	(void)shell_state;
	cmd_list = NULL;
	current_cmd = NULL;
	while (token_list)
	{
		if (!process_token(&token_list, &current_cmd, &cmd_list))
		{
			free_cmd_list(&cmd_list);
			return (NULL);
		}
		token_list = token_list->next;
	}
	return (cmd_list);
}
