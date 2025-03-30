/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:11 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/30 15:30:46 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "command.h"
# include "error_handler.h"
# include "shell_state.h"

# define INITIAL_TOKEN_BUF_SIZE 64

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_HEREDOC_DELIMITER
}	t_token_type;

typedef struct s_token	t_token;
struct s_token
{
	t_token_type	type;
	char			*value;
	int				len;
	int				capacity;
};

typedef enum e_state
{
	STATE_NONE,
	STATE_WORD,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE,
	STATE_REDIR_IN,
	STATE_REDIR_OUT,
	STATE_HEREDOC_DELIM
}	t_state;

typedef bool			(*t_token_state_handler)(t_state *, t_list **, char);

typedef enum e_expand_state
{
	EXPAND_WORD,
	EXPAND_IN_SINGLE_QUOTE,
	EXPAND_IN_DOUBLE_QUOTE,
	EXPAND_IN_ENV
}	t_expand_state;

typedef struct s_expand	t_expand;
typedef bool			(*t_expand_state_handler)(t_expand *, char, t_list *);

struct	s_expand
{
	t_expand_state	state;
	t_expand_state	prev_state;
	t_shell_state	*shell_state;
	char			*value;
	int				len;
	int				capacity;
	char			*env_key;
	int				env_key_len;
	int				env_key_capacity;
};

typedef struct s_parser	t_parser;
struct s_parser
{
	char		*line;
	t_list		*(*parse)(t_parser *, t_shell_state *);
};

// parser
t_parser		*create_parser(t_shell_state *sh_state);
void			free_parser(t_parser *parser);
char			*ft_readline(t_shell_state *sh_state, const char *prompt);
t_list			*parse(t_parser *parser, t_shell_state *shell_state);
t_list			*parse_tokens(t_list *token_list, t_shell_state *shell_state);
bool			parse_redirection(t_list **token_list, t_cmd *cmd_content);
bool			parse_command_args(t_token *token_content, t_cmd *cmd_content);

// tokenizer
t_list			*tokenize_line(const char *line);
t_list			*create_token(t_token_type type, const char value);
void			free_token(void *token);
t_token_type	get_token_type(char c);
bool			add_new_token(t_list **head, char c, t_state *state);
bool			append_char_to_token(t_list **head, char c);
// tokenizer state handler
bool			handle_tokenize_state(t_state *state, t_list **head, char c);
bool			tokenize_state_none(t_state *state, t_list **head, char c);
bool			tokenize_state_word(t_state *state, t_list **head, char c);
bool			tokenize_state_in_single_quote(t_state *state \
										, t_list **head, char c);
bool			tokenize_state_in_double_quote(t_state *state \
										, t_list **head, char c);
bool			tokenize_state_in_redir_in(t_state *state \
										, t_list **head, char c);
bool			tokenize_state_in_redir_out(t_state *state \
										, t_list **head, char c);
bool			tokenize_state_heredoc_delim(t_state *state \
										, t_list **head, char c);

// expander
t_expand		*create_expand(t_shell_state *shell_state);
void			free_expand(t_expand *expand_ctx);
bool			resolve_pending_expansion(t_expand *expand_ctx \
										, t_list *env_list);
bool			initialize_env_key(t_expand *expand_ctx);
bool			expansion(t_list **token_list, t_shell_state *shell_state);
bool			dispatch_expand_state_handler(t_expand *expand_ctx \
										, char c, t_list *env_list);
bool			expand_state_in_env(t_expand *expand_ctx \
										, char c, t_list *env_list);
bool			replace_with_expanded_token(t_list **current \
										, t_list *prev, t_expand **expand_ctx);
bool			is_empty_token(t_token *token);
void			remove_token_node(t_list **token_list, t_list *to_remove \
										, t_list *prev);
// expander string handler
bool			append_char_to_env_key(t_expand *expand_ctx, char c);
bool			append_char_to_str(t_expand *expand_ctx, char c);
bool			append_last_status_to_str(t_expand *expand_ctx);
bool			set_env_value_to_str(t_expand *expand_ctx, t_list *env_list);

// utils
bool			ft_is_space(char c);

#endif
