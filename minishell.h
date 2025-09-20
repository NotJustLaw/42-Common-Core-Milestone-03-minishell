/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:38 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/20 18:12:51 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <stdarg.h>
# include <fcntl.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <aio.h>
# include <stddef.h>	
# include <sys/time.h>
# include <signal.h>


	typedef struct s_command
	{
		char		**args;
		int			input_fd;
		int			output_fd;
		char 		*temp_env;
		struct s_command *next;
		char                *infile;
		char                *outfile;
		int                    append;
		int                    heredoc;
		char                *delimiter;
	}	t_command;

typedef struct s_shell
{	
	char	**envp;
	t_command	*commands;
	int		exit_status;
	int		is_running;
}	t_shell;

//main
t_shell	*prog_data();

t_command *parser(char *line);
void	append_commands(t_command *new_node);
void	free_commands(t_command *commands);
bool    pipe_checker(char *line);
bool	redirection_checker(char *line);
bool	expansion_chekcker(const char *line);
void	full_sighandler();
void	sigint_handler(int sig);
void 	reset_signals_in_child(void);
bool	we_need_space(char *line);
// static void	*free_split(char **str);
char	*ft_strndup(const char *s1, size_t n);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int 	strncat_realloc(char **dest, const char *source, size_t n);
char 	*expand_argument(const char *arg);
void 	expansion_trade(void);
void	check_redirs(void);
bool 	ft_strchr2(const char *s, int c);
void	*free_split(char **str);


#endif