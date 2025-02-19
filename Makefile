# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 14:58:49 by yuuchiya          #+#    #+#              #
#    Updated: 2025/02/19 19:55:15 by yuuchiya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=	minishell
COMMON_SRCS		:=	main.c \
					command.c \
					ft_cd.c \
					ft_echo.c \
					ft_env.c \
					ft_exit.c \
					ft_export.c \
					ft_pwd.c \
					ft_unset.c \
					builtins.c \
					error_handler.c \
					executor.c \
					front_desk.c \
					parser.c \
					tokenizer.c \

OBJ_DIR			:=	./obj
OBJS			:=	$(COMMON_SRCS:%.c=$(OBJ_DIR)/%.o)
LIBFT_DIR		:=	libft
LD_FLAGS		:=	-L$(LIBFT_DIR)
LD_LIBS			:=	-lft -lreadline 
INC				:=	-Iincludes -I$(LIBFT_DIR) -MMD -MP
CC				:=	cc
CFLAGS			:=	-Wall -Wextra -Werror

vpath %.c	./srcs:./srcs/commands:./srcs/commands/redirections:./srcs/error_handler:./srcs/executor:./srcs/executor/builtins:./srcs/parser:./srcs/front_desk:
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

clean:
	$(RM) -r $(OBJS) $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean:	clean
	$(RM) $(NAME) 
	$(MAKE) fclean -C $(LIBFT_DIR)

re:	fclean all

.PHONY:	all clean fclean re