# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 14:58:49 by yuuchiya          #+#    #+#              #
#    Updated: 2025/03/28 18:07:42 by yuuchiya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=	minishell
COMMON_SRCS		:=	shell_core_main.c \
					command_processor.c \
					command.c \
					redirections.c \
					heredoc_handler.c \
					ft_cd.c \
					ft_echo.c \
					ft_env.c \
					ft_exit.c \
					ft_export.c \
					ft_pwd.c \
					ft_unset.c \
					builtins.c \
					error_handler.c \
					error_handler_util.c \
					executor.c \
					command_execution.c \
					executor_flow.c \
					executor_pipe_handler.c \
					executor_redirection_handler.c \
					executor_process_handler.c \
					wait_comand_handler.c \
					parser.c \
					parser_readline.c \
					parser_token.c \
					parser_cmd.c \
					parser_redirection.c \
					tokenizer.c \
					tokenizer_util.c \
					tokenizer_state_normal.c \
					tokenizer_state_quote.c \
					tokenizer_state_redirection.c \
					expander.c \
					expander_token_util.c \
					expander_buffer_manager.c \
					expander_state_handler.c \
					expander_env_handler.c \
					expander_constructor.c \
					expander_filter.c \
					expander_empty_filter.c \
					expander_quotes_filter.c \
					expander_variable_filter.c \
					environment.c \
					environment_util.c \
					environment_access.c \
					shell_state.c \
					fd_syscall_wrapper.c \
					process_syscall_wrapper.c \
					signal_handler.c \
					signal_handler_util.c

OBJ_DIR			:=	./obj
OBJS			:=	$(COMMON_SRCS:%.c=$(OBJ_DIR)/%.o)
LIBFT_DIR		:=	libft
LD_FLAGS		:=	-L$(LIBFT_DIR)
LD_LIBS			:=	-lft -lreadline 
INC				:=	-Iincludes -I$(LIBFT_DIR) -MMD -MP
CC				:=	cc
CFLAGS			:=	-Wall -Wextra -Werror

vpath %.c	./srcs:./srcs/commands:./srcs/commands/redirections:./srcs/error_handler:./srcs/executor:./srcs/executor/builtins:./srcs/parser/tokenize_process:./srcs/parser/expand_process:./srcs/parser/parse_process:./srcs/environment:./srcs/shell_state:./srcs/signals:./srcs/syscall_wrapper
vpath %.sh	./tests

$(NAME) : $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LD_FLAGS) $(LD_LIBS) -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT_DIR)/libft.a: $(LIBFT_DIR)
	$(MAKE) complete -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

all:	$(NAME)

test:	all
	./tests/test.sh

coverage:
	$(MAKE) CFLAGS="-g -O0 -fprofile-arcs -ftest-coverage -Wall -Wextra -Werror" LDFLAGS="-fprofile-arcs -ftest-coverage -L$(LIBFT_DIR) -lft -lcriterion -lgcov --coverage -lreadline"

clean:
	$(RM) -r $(OBJS) $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean:	clean
	$(RM) $(NAME) 
	$(MAKE) fclean -C $(LIBFT_DIR)

re:	fclean all

.PHONY:	all clean fclean re
