/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/04 17:49:36 by yuuchiya         ###   ########.fr       */
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
		if (token_content->type == TOKEN_REDIR_IN || token_content->type == TOKEN_REDIR_OUT \
			|| token_content->type == TOKEN_REDIR_APPEND)
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

bool	expand_state_in_env(t_expand_state *state, t_list **head, char c, t_list *env_list)
{
	static char	*env_key;
	char		*env_value;
	t_env		*env_content;
	t_token		*token;

	if (c == '\"')
	{
		env_value = get_env_value(env_list, env_key);
		if (env_value)
			append_string_to_token(head, env_value);
		*state = EXPAND_IN_DOUBLE_QUOTE;
	}
	else if (c == '\'')
	{
		env_value = get_env_value(env_list, env_key);
		if (env_value)
			append_string_to_token(head, env_value);
		*state = EXPAND_IN_SINGLE_QUOTE;
	}
	else if (c == '$')
		*state = EXPAND_WORD;
	else if (c == '?')
	{
		// TODO append exit status
	}
	else
	{
		if (!env_key)
			env_key = ft_strdup(&c);
		else
		{
			if (!append_char_to_str(&env_key, c))
				return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
		}
	}
	return (true);
}

bool	expand_state_in_double_quote(t_expand_state *state, t_list **head, char c, t_list *env_list)
{
	if (c == '\"')
		*state = EXPAND_WORD;
	else if (c == '$')
		*state = EXPAND_IN_ENV;
	else
	{
		if (!append_char_to_token(head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	}
	return (true);
}

bool	expand_state_in_single_quote(t_expand_state *state, t_list **head, char c, t_list *env_list)
{
	if (c == '\'')
		*state = EXPAND_WORD;
	else
	{
		if (!append_char_to_token(head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	}
	return (true);
}

bool	expand_state_word(t_expand_state *state, t_list **head, char c, t_list *env_list)
{
	if (c == '\'')
		*state = EXPAND_IN_SINGLE_QUOTE;
	else if (c == '\"')
		*state = EXPAND_IN_DOUBLE_QUOTE;
	else if (c == '$')
		*state = EXPAND_IN_ENV;
	else
	{
		if (!append_char_to_token(head, c))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	}
	return (true);
}

bool	expand_word(t_list **current, t_list *prev, t_list *env_list)
{
	t_list			*head;
	t_expand_state	state;
	t_token			*token;
	t_list			*new_token;
	int				i;

	head = *current;
	state = EXPAND_WORD;
	token = (t_token *)(head->content);
	new_token = create_token(TOKEN_WORD, "");
	if (!new_token)
		return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer", "malloc failed"), false);
	i = 0;
	while (token->value[i])
	{
		// ft_printf(STDOUT_FILENO, "line[%d]:%c\n", i, line[i]);
		if (state == EXPAND_WORD)
		{
			if (!expand_state_word(&state, &new_token, token->value[i], env_list))
				return (ft_lstclear(&head, free_token), false);
		}
		else if (state == EXPAND_IN_SINGLE_QUOTE)
		{
			if (!expand_state_in_single_quote(&state, &new_token, token->value[i], env_list))
				return (ft_lstclear(&head, free_token), false);
		}
		else if (state == EXPAND_IN_DOUBLE_QUOTE)
		{
			if (!expand_state_in_double_quote(&state, &new_token, token->value[i], env_list))
				return (ft_lstclear(&head, free_token), false);
		}
		else if (state == EXPAND_IN_ENV)
		{
			if (!expand_state_in_env(&state, &new_token, token->value[i], env_list))
				return (ft_lstclear(&head, free_token), false);
		}
		i++;
	}
	prev->next = new_token;
	new_token->next = head->next;
	ft_lstdelone(&head, free_token);
	*current = new_token;
	return (true);
}

bool	expansion(t_list *token_list, t_list *env_list)
{
	t_list	*current;
	t_list	*prev;
	t_token	*token;
	t_env	*env;
	char	*value;

	current = token_list;
	prev = NULL;
	while (current)
	{
		token = (t_token *)(current->content);
		if (token->type == TOKEN_WORD)
		{
			expand_word(&current, prev, env_list);
		}
		prev = current;
		current = current->next;
	}
	return (true);
}

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
