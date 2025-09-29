# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/06 15:27:34 by hcarrasq          #+#    #+#              #
#    Updated: 2025/09/29 21:30:39 by henrique-re      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Directories
SRCS_DIR = srcs
EXEC_DIR = execution_module
OBJS_DIR = objs
LIBFT_DIR = libraries/libft
PIPEX_DIR = libraries/pipex1/src
PIPEX_SRCS = pipex_bonus.c pipex_utils.c pipex_bonus_utils.c \
             pipex_utils_extra.c files_handler.c get_next_line_bonus.c \
             get_next_line_utils_bonus.c pipex_heredoc_utils.c

PIPEX_OBJS = $(addprefix $(PIPEX_DIR)/, $(PIPEX_SRCS:.c=.o))

# Source files
SRCS_FILES = main.c parsing.c commands_utils.c checkers.c signal_handling.c space_utils.c expansions.c redirections.c exapansion_helpers.c expand_argument.c redirections_helpers.c parsing_helpers.c
EXEC_FILES = builtin_commands.c builtin_commands_2.c builtin_helper.c env_manager.c env_manager_utils.c execution_builtin.c execution_handler.c execution.c heredoc_exec.c heredoc_utils.c

# Full paths
SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))
EXEC = $(addprefix $(EXEC_DIR)/, $(EXEC_FILES))
ALL_SRCS = $(SRCS) $(EXEC)
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(ALL_SRCS:.c=.o)))

# Compiler and flags
CC = cc
CFLAGS = -g -Wall -Wextra -Werror

# Build targets
all: $(NAME)

$(NAME): $(OBJS) $(PIPEX_OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(MAKE) -C $(PIPEX_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(PIPEX_OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(EXEC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(PIPEX_DIR) clean
	@rm -rf $(OBJS_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(PIPEX_DIR) fclean
	@rm -f $(NAME)

re: fclean all

r:
	make re && clear && ./$(NAME)

.PHONY: all clean fclean re r
