/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:11 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/05 17:18:39 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H


# include "minishell.h"
# include "command.h"
# include "error_handler.h"

# define META_CHARACTER " \t\n|;><"
# define INITIAL_TOKEN_BUF_SIZE 64

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC
}	t_token_type;

typedef struct s_token	t_token;
struct s_token
{
	t_token_type	type;
	char			*value;
	int				len;
	int				capacity;
};

typedef enum e_state {
	STATE_NONE,
	STATE_WORD,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE,
	STATE_REDIR_IN,
	STATE_REDIR_OUT
}	t_state;

typedef enum e_expand_state
{
	EXPAND_WORD,
	EXPAND_IN_SINGLE_QUOTE,
	EXPAND_IN_DOUBLE_QUOTE,
	EXPAND_IN_ENV
}	t_expand_state;


typedef struct s_parser	t_parser;
struct s_parser
{
	char		*line;
	t_list		*(*parse)(t_parser *, t_error_handler *, t_list *);
};

t_parser	*create_parser(t_error_handler *err_handler);
void		free_parser(t_parser *parser);

t_list		*tokenize_line(const char *line);
t_list		*create_token(t_token_type type, const char value);
void		free_token(void *token);

#endif
