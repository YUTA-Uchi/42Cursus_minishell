/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/02 19:07:14 by yuuchiya         ###   ########.fr       */
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

t_redir_type	get_redir_type(t_token_type token_type, bool is_double_char)
{
	if (is_double_char)
	{
		if (token_type == TOKEN_REDIR_IN)
			return (REDIR_HEREDOC);
		else
			return (REDIR_APPEND);
	}
	else
	{
		if (token_type == TOKEN_REDIR_IN)
			return (REDIR_IN);
		else
			return (REDIR_OUT);
	}
}

t_list	*parse_tokens(t_list *tokens, t_error_handler *err_handler)
{
	t_list	*head;
	t_list	*current_cmd;
	t_token	*token_content;
	t_cmd	*cmd_content;
	t_list	*redirection;

	(void)err_handler;
	head = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		token_content = (t_token *)(tokens->content);
		if (token_content->type == TOKEN_PIPE)
		{
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
		// ft_printf(STDOUT_FILENO, "token:%s:%d\n", token_content->value, token_content->type);
		if (token_content->type == TOKEN_REDIR_IN || token_content->type == TOKEN_REDIR_OUT)
		{
			// ft_printf(STDOUT_FILENO, "redir:%s:%d\n", token_content->value, token_content->type);
			// ft_printf(STDOUT_FILENO, "redir:%s\n", ((t_token *)(tokens->next->content))->value);
			if (tokens->next && ((t_token *)(tokens->next->content))->type == TOKEN_WORD)
			{
				redirection = create_redirection(((t_token *)(tokens->next->content))->value \
										, get_redir_type(token_content->type, false));
				// ft_printf(STDOUT_FILENO, "redir:%s:%s\n", ((t_token *)(tokens->next->content))->value 
				// 		, ((t_redirection *)(redirection->content))->file);
				if (!redirection)
					return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "malloc failed"), NULL);
				ft_lstadd_back(&(cmd_content->redirections), redirection);
				tokens = tokens->next;
			}
			else if (tokens->next && ((t_token *)(tokens->next->content))->type == token_content->type)
			{
				tokens = tokens->next;
				if (tokens->next && ((t_token *)(tokens->next->content))->type == TOKEN_WORD)
				{
					redirection = create_redirection(((t_token *)(tokens->next->content))->value \
										, get_redir_type(token_content->type, true));
					if (!redirection)
						return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "malloc failed"), NULL);
					ft_lstadd_back(&(cmd_content->redirections), redirection);
					tokens = tokens->next;
				}
				else
					return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "syntax error"), NULL);
			}
			else
				return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "syntax error"), NULL);
		}
		else if (token_content->type == TOKEN_WORD)
		{
			if (!cmd_content->cmd_name)
				cmd_content->cmd_name = ft_strdup(token_content->value);
			if (!cmd_content->cmd_name || !add_arg(&(cmd_content->args), ft_strdup(token_content->value)))
				return (free_cmd_list(&head), ft_printf(STDERR_FILENO, "%s: %s\n", "parser", "malloc failed"), NULL);
		}
		tokens = tokens->next;
	}
	return (head);
}

static char	*ft_readline(t_error_handler *error_handler, const char *prompt)
{
	char	*line;

	rl_outstream = stderr;
	line = readline(prompt);
	if (!line)
	{
		// TODO error handling
		return (ft_printf(STDERR_FILENO, "input again!\n"), ft_readline(error_handler, prompt));
	}
	if (*line)
		add_history(line);
	return (line);
}

// bool env_expansion(t_list *token_list, t_list *env_list)
// {
// 	t_list	*current;
// 	t_token	*token;
// 	t_env	*env;
// 	char	*value;

// 	current = token_list;
// 	while (current)
// 	{
// 		token = (t_token *)(current->content);
// 		if (token->type == TOKEN_WORD)
// 		{
// 			env = find_env(env_list, token->value);
// 			if (env)
// 			{
// 				value = ft_strdup(env->value);
// 				if (!value)
// 					return (false);
// 				free(token->value);
// 				token->value = value;
// 			}
// 		}
// 		current = current->next;
// 	}
// 	return (true);
// }

t_list	*parse(t_parser *parser, t_error_handler *error_handler, t_list *env_list)
{
	char	*line;
	t_list	*cmd;
	t_list	*token_list;

	line = parser->line;
	token_list = tokenize_line(line);
	if (!token_list)
		return (NULL);
	(void)env_list;
	// TODO env_expansion(token_list, env_list);
	// if (!env_expansion(token_list, env_list))
	// 	return (ft_lstclear(&token_list, free_token), NULL);
	cmd = parse_tokens(token_list, error_handler);
	if (!cmd)
		return (ft_lstclear(&token_list, free_token), NULL);
	ft_lstclear(&token_list, free_token);
	return (cmd);
}

t_parser	*create_parser(t_error_handler *error_handler)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->parse = parse;
	parser->line = ft_readline(error_handler, PROMPT);
	return (parser);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}
