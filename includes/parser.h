/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:11 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/21 14:35:10 by yuuchiya         ###   ########.fr       */
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
	TOKEN_REDIR_OUT
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
	STATE_IN_DOUBLE_QUOTE
}	t_state;


typedef struct s_parser	t_parser;
struct s_parser
{
	char	*line;
	t_list	*(*parse)(t_parser *, t_error_handler *);
};

t_parser	*create_parser(t_error_handler *err_handler);
void		free_parser(t_parser *parser);

t_list		*tokenize_line(const char *line);

#endif
