# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 14:58:49 by yuuchiya          #+#    #+#              #
#    Updated: 2025/02/17 15:24:49 by yuuchiya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=	minishell
COMMON_SRCS		:=	main.c

OBJ_DIR			:=	./obj
OBJS			:=	$(COMMON_SRCS:%.c=$(OBJ_DIR)/%.o)
# LIBFT_DIR		:=	libft
# LD_FLAGS		:=	-L$(LIBFT_DIR)
LD_LIBS			:=	-lreadline
INC				:=	-Iincludes -MMD -MP
CC				:=	cc
CFLAGS			:=	-Wall -Wextra -Werror

vpath %.c	./srcs
vpath %.sh	./tests

$(NAME) : $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LD_LIBS) -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# $(LIBFT_DIR)/libft.a: $(LIBFT_DIR)
# 	$(MAKE) complete -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

all:	$(NAME)

test:	all
	./tests/test.sh

clean:
	$(RM) -r $(OBJS) $(OBJ_DIR)
# $(MAKE) clean -C $(LIBFT_DIR)

fclean:	clean
	$(RM) $(NAME) 
#	$(MAKE) fclean -C $(LIBFT_DIR)

re:	fclean all

.PHONY:	all clean fclean re