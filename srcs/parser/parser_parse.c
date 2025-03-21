/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:04:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/21 17:09:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

bool	add_arg(char ***args, char *arg)
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
		return (false);
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

t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else
		return (REDIR_HEREDOC);
}

bool	add_cmd_node_to_list(t_list **cmd_list, t_list **current_cmd)
{
	if (!(*current_cmd))
	{
		*current_cmd = create_cmd();
		if (!(*current_cmd))
			return (print_const_error(MALLOCF, 0), NULL);
		ft_lstadd_back(cmd_list, *current_cmd);
	}
	return (true);
}

bool	parse_command_args(t_token *token_content, t_cmd *cmd_content)
{
	if (token_content->type == TOKEN_WORD)
	{
		if (!cmd_content->cmd_name)
			cmd_content->cmd_name = ft_strdup(token_content->value);
		if (!cmd_content->cmd_name \
			|| !add_arg(&(cmd_content->args), ft_strdup(token_content->value)))
			return (print_const_error(MALLOCF, 0), NULL);
	}
	return (true);
}

bool	parse_redirection(t_list **token_list, t_cmd *cmd_content)
{
	t_token	*token_content;
	t_token	*next_token_content;
	t_list	*redirection;

	token_content = (t_token *)((*token_list)->content);
	if (token_content->type >= TOKEN_REDIR_IN)
	{
		if ((*token_list)->next)
			next_token_content = (t_token *)((*token_list)->next->content);
		if ((*token_list)->next \
			&& next_token_content->type == TOKEN_WORD)
		{
			redirection = create_redirection(next_token_content->value \
									, get_redir_type(token_content->type));
			if (!redirection)
				return (false);
			ft_lstadd_back(&(cmd_content->redirections), redirection);
			*token_list = (*token_list)->next;
		}
		else
			return (print_const_error(SYNERR, 0), false);
	}
	return (true);
}

t_list	*parse_tokens(t_list *token_list, t_shell_state *shell_state)
{
	t_list	*cmd_list;
	t_list	*current_cmd;
	t_token	*token_content;
	t_cmd	*cmd_content;

	(void)shell_state;
	cmd_list = NULL;
	current_cmd = NULL;
	while (token_list)
	{
		token_content = (t_token *)(token_list->content);
		if (token_content->type == TOKEN_PIPE)
		{
			if (!current_cmd)
				return (free_cmd_list(&cmd_list) \
						, print_const_error(SYNERR_NEAR_PIPE, 0), NULL);
			current_cmd = NULL;
			token_list = token_list->next;
			continue ;
		}
		if (!add_cmd_node_to_list(&cmd_list, &current_cmd))
			return (free_cmd_list(&cmd_list), NULL);
		cmd_content = (t_cmd *)(current_cmd->content);
		if (!parse_redirection(&token_list, cmd_content))
			return (free_cmd_list(&cmd_list), NULL);
		if (!parse_command_args(token_content, cmd_content))
			return (free_cmd_list(&cmd_list), NULL);
		token_list = token_list->next;
	}
	return (cmd_list);
}
