/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:34:43 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/06 18:06:49 by yuuchiya         ###   ########.fr       */
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
			// ft_printf(STDOUT_FILENO, "cmd_name:%s\n", cmd_content->cmd_name);
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

bool	append_char_to_env_key(t_expand *expand, char c)
{
	char	*new_key;

	if (expand->env_key_len + 1 >= expand->env_key_capacity)
	{
		expand->env_key_capacity *= 2;
		new_key = malloc(sizeof(char) * (expand->env_key_capacity));
		if (!new_key)
			return (false);
		ft_strlcpy(new_key, expand->env_key, expand->env_key_len + 1);
		free(expand->env_key);
		expand->env_key = new_key;
	}
	expand->env_key[expand->env_key_len++] = c;
	expand->env_key[expand->env_key_len] = '\0';
	return (true);
}

bool	append_char_to_str(t_expand *expand, char c)
{
	char	*new_value;

	if (expand->len + 1 >= expand->capacity)
	{
		expand->capacity *= 2;
		new_value = malloc(sizeof(char) * (expand->capacity));
		if (!new_value)
			return (false);
		ft_strlcpy(new_value, expand->value, expand->len + 1);
		free(expand->value);
		expand->value = new_value;
	}
	expand->value[expand->len++] = c;
	expand->value[expand->len] = '\0';
	return (true);
}

bool	append_last_status_to_str(t_expand *expand)
{
	char	*status_str;
	int		i;

	status_str = ft_itoa(expand->shell_state->last_status);
	if (!status_str)
		return (false);
	i = 0;
	while (status_str[i])
	{
		if (!append_char_to_str(expand, status_str[i]))
		{
			free(status_str);
			return (false);
		}
		i++;
	}
	free(status_str);
	return (true);
}

bool	set_env_value_to_str(t_expand *expand_context, t_list *env_list)
{
	char	*env_value;
	int		i;

	env_value = get_env_value(env_list, expand_context->env_key);
	if (env_value)
	{
		i = 0;
		while (env_value[i])
		{
			if (!append_char_to_str(expand_context, env_value[i++]))
				return (false);
		}
	}
	return (true);
}

bool	expand_state_in_env(t_expand *expand_context, char c, t_list *env_list)
{
	if (c == '\"')
	{
		if (!set_env_value_to_str(expand_context, env_list))
			return (false);
		expand_context->state = expand_context->prev_state;
	}
	else if (c == '\'')
	{
		if (!set_env_value_to_str(expand_context, env_list))
			return (false);
		if (expand_context->prev_state == EXPAND_IN_DOUBLE_QUOTE)
		{
			if (!append_char_to_str(expand_context, c))
				return (false);
		}
		expand_context->state = expand_context->prev_state;
	}
	else if (c == '$')
		expand_context->state = EXPAND_WORD;
	else if (c == '?')
	{
		if (!append_last_status_to_str(expand_context))
			return (false);
		expand_context->state = expand_context->prev_state;
	}
	else
	{
		if (!append_char_to_env_key(expand_context, c))
			return (false);
	}
	return (true);
}

bool	expand_state_in_double_quote(t_expand *expand_context, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\"')
	{
		expand_context->state = EXPAND_WORD;
		expand_context->prev_state = EXPAND_WORD;
	}
	else if (c == '$')
	{
		expand_context->state = EXPAND_IN_ENV;
		expand_context->prev_state = EXPAND_IN_DOUBLE_QUOTE;
	}
	else
	{
		if (!append_char_to_str(expand_context, c))
			return (false);
	}
	return (true);
}

bool	expand_state_in_single_quote(t_expand *expand_context, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\'')
		expand_context->state = EXPAND_WORD;
	else
	{
		if (!append_char_to_str(expand_context, c))
			return (false);
	}
	return (true);
}

bool	expand_state_word(t_expand *expand_context, char c, t_list *env_list)
{
	(void)env_list;
	if (c == '\'')
		expand_context->state = EXPAND_IN_SINGLE_QUOTE;
	else if (c == '\"')
		expand_context->state = EXPAND_IN_DOUBLE_QUOTE;
	else if (c == '$')
		expand_context->state = EXPAND_IN_ENV;
	else
	{
		if (!append_char_to_str(expand_context, c))
			return (false);
	}
	return (true);
}

bool	handle_expand_state(t_expand *expand, char c, t_list *env_list)
{
	const t_expand_state_handler	expand_state_handler[4] = {\
		expand_state_word, \
		expand_state_in_single_quote, \
		expand_state_in_double_quote, \
		expand_state_in_env
	};

	return (expand_state_handler[expand->state](expand, c, env_list));
}

t_expand	*create_expand(t_shell_state *shell_state)
{
	t_expand	*expand;

	expand = malloc(sizeof(t_expand));
	if (!expand)
		return (NULL);
	expand->state = EXPAND_WORD;
	expand->prev_state = EXPAND_WORD;
	expand->shell_state = shell_state;
	expand->value = malloc(sizeof(char) * 1);
	if (!expand->value)
		return (free(expand), NULL);
	expand->value[0] = '\0';
	expand->len = 0;
	expand->capacity = 1;
	expand->env_key = malloc(sizeof(char) * 1);
	if (!expand->env_key)
		return (free(expand->value), free(expand), NULL);
	expand->env_key[0] = '\0';
	expand->env_key_len = 0;
	expand->env_key_capacity = 1;
	return (expand);
}

void	free_expand(t_expand *expand)
{
	if (expand->value)
		free(expand->value);
	if (expand->env_key)
		free(expand->env_key);
	free(expand);
}


bool	finalize_expand(t_expand *expand, t_list *env_list)
{
	if (expand->state == EXPAND_IN_ENV)
	{
		if (!set_env_value_to_str(expand, env_list))
			return (false);
	}
	return (true);
}

t_list	*set_token_value(t_list *token, t_expand *expand)
{
	t_token	*token_content;

	if (!token || !expand->value || !token->content)
		return (NULL);
	token_content = (t_token *)(token->content);
	free(token_content->value);
	token_content->value = ft_strdup(expand->value);
	if (!token_content->value)
		return (NULL);
	token_content->len = expand->len;
	token_content->capacity = expand->capacity;
	return (token);
}

bool	expand_word(t_list **current, t_list *prev, t_list *env_list, t_shell_state *shell_state)
{
	t_expand		*expand;
	t_token			*token;
	t_list			*new_token;
	int				i;

	token = (t_token *)((*current)->content);
	expand = create_expand(shell_state);
	i = 0;
	while (token->value[i])
	{
		if (!handle_expand_state(expand, token->value[i++], env_list))
			return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer1", "malloc failed"), false);
	}
	if (!finalize_expand(expand, env_list))
		return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer2", "malloc failed"), false);
	new_token = set_token_value(create_token(TOKEN_WORD, '\0'), expand);
	if (!new_token)
		return (ft_printf(STDERR_FILENO, "%s: %s\n", "tokenizer3", "malloc failed"), false);
	free_expand(expand);
	if (prev)
		prev->next = new_token;
	new_token->next = (*current)->next;
	ft_lstdelone((*current), free_token);
	*current = new_token;
	return (true);
}

bool	expansion(t_list **token_list, t_list *env_list, t_shell_state *shell_state)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = *token_list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)(current->content);
		if (token->type == TOKEN_WORD)
		{
			if (!expand_word(&current, prev, env_list, shell_state))
				return (false);
		}
		if (!prev)
			*token_list = current;
		prev = current;
		current = next;
	}
	return (true);
}

t_list	*parse(t_parser *parser, t_error_handler *error_handler, t_list *env_list, t_shell_state *shell_state)
{
	char	*line;
	t_list	*cmd;
	t_list	*token_list;

	line = parser->line;
	token_list = tokenize_line(line);
	if (!token_list)
		return (NULL);
	if (!expansion(&token_list, env_list, shell_state))
		return (ft_lstclear(&token_list, free_token), NULL);
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
