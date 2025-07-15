# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/06 15:27:34 by hcarrasq          #+#    #+#              #
#    Updated: 2025/07/15 12:00:46 by hcarrasq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_FILES = main.c parsing.c commands_utils.c checkers.c signal_handling.c space_utils.c
SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

SRCS_DIR = srcs

OBJS_DIR = objs

OBJS	= $(addprefix $(OBJS_DIR)/, $(SRCS_FILES:.c=.o))

CFLAGS = -g -Wall -Wextra -Werror
CC = cc

all:	$(NAME)

$(NAME): $(OBJS) $(OBJS_DIR)
	$(MAKE) -C ./libft
	$(CC) $(CFLAGS) $(OBJS) libft/libft.a -Llibft -lft -lreadline -o $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		@cd libft && make clean
		@rm -rf $(OBJS)

fclean:		clean
		@cd libft && make fclean
		@rm -rf $(NAME)

re:		fclean 	all

r:
	make re && clear && ./minishell

.PHONY : all clean fclean re