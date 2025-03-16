/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:04:17 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/12 12:18:22 by yuuchiya         ###   ########.fr       */
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

t_list	*parse_tokens(t_list *tokens, t_shell_state *shell_state)
{
	t_list	*head;
	t_list	*current_cmd;
	t_token	*token_content;
	t_cmd	*cmd_content;
	t_list	*redirection;

	(void)shell_state;
	head = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		token_content = (t_token *)(tokens->content);
		if (token_content->type == TOKEN_PIPE)
		{
			if (!current_cmd)
				return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "syntax error near the '|'"), NULL);
			current_cmd = NULL;
			tokens = tokens->next;
			continue ;
		}
		if (!current_cmd)
		{
			current_cmd = create_cmd();
			if (!current_cmd)
				return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "malloc failed"), NULL);
			if (!head)
				head = current_cmd;
			else
				ft_lstadd_back(&head, current_cmd);
		}
		cmd_content = (t_cmd *)(current_cmd->content);
		if (token_content->type == TOKEN_REDIR_IN \
			|| token_content->type == TOKEN_REDIR_OUT \
			|| token_content->type == TOKEN_REDIR_APPEND \
			|| token_content->type == TOKEN_REDIR_HEREDOC)
		{
			if (tokens->next && ((t_token *)(tokens->next->content))->type == TOKEN_WORD)
			{
				redirection = create_redirection(((t_token *)(tokens->next->content))->value \
										, get_redir_type(token_content->type));
				if (!redirection)
					return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "malloc failed"), NULL);
				ft_lstadd_back(&(cmd_content->redirections), redirection);
				tokens = tokens->next;
			}
			else
				return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "syntax error"), NULL);
		}
		else if (token_content->type == TOKEN_WORD)
		{
			if (!cmd_content->cmd_name)
				cmd_content->cmd_name = ft_strdup(token_content->value);
			if (!cmd_content->cmd_name \
				|| !add_arg(&(cmd_content->args), ft_strdup(token_content->value)))
				return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "malloc failed"), NULL);
		}
		tokens = tokens->next;
	}
	return (head);
}

t_list	*parse(t_parser *parser, t_shell_state *shell_state)
{
	char	*line;
	t_list	*cmd;
	t_list	*token_list;

	line = parser->line;
	token_list = tokenize_line(line);
	if (!token_list)
		return (NULL);
	if (!expansion(&token_list, shell_state))
		return (ft_lstclear(&token_list, free_token), NULL);
	cmd = parse_tokens(token_list, shell_state);
	if (!cmd)
		return (ft_lstclear(&token_list, free_token), NULL);
	ft_lstclear(&token_list, free_token);
	return (cmd);
}
