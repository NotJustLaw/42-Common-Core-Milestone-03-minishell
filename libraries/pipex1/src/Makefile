# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/30 18:52:34 by justlaw           #+#    #+#              #
#    Updated: 2025/09/06 16:56:03 by justlaw          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME       = pipex_objs
CC         = cc
CFLAGS     = -Wall -Wextra -Werror

LIBFT_DIR  = ../../libft
LIBFT      = $(LIBFT_DIR)/libft.a
SRCS =	pipex.c \
		pipex_bonus.c \
		pipex_utils.c \
		pipex_bonus_utils.c \
		pipex_utils_extra.c \
		files_handler.c \
		get_next_line_bonus.c \
		get_next_line_utils_bonus.c \
		pipex_heredoc_utils.c 

OBJS       = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Pipex object files ready"

# Build libft first (into libft.a), so we can link against it
$(LIBFT):
	   $(MAKE) -C $(LIBFT_DIR)

# Compile every .c file in SRCS into a .o
%.o: %.c pipex.h get_next_line_bonus.h
	   $(CC) $(CFLAGS) -c $< -o $@

clean:
	   rm -f $(OBJS)
	   $(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	   rm -f $(NAME)
	   $(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
