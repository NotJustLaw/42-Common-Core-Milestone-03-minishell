/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:38 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/10/01 18:42:26 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define MINISHELL_H

# include "libraries/libft/libft.h"
# include "libraries/pipex1/src/pipex.h"
# include "execution_module/env_manager.h"
# include <stddef.h>
# include <pthread.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <stdarg.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <aio.h>
# include <stddef.h>	
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/ioctl.h>

typedef struct s_heredoc
{
	char				*delim;
	int					expand;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_command
{
	char				**args;
	char				*dlm;
	int					input_fd;
	int					output_fd;
	struct s_command	*next;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	int					heredoc_expand;
	char				*delimiter;
	int					redirection_failed;
	t_heredoc			*heredocs;
}	t_command;

typedef struct s_shell
{	
	char				**envp;
	t_command			*commands;
	int					exit_status;
	int					is_running;
	sig_atomic_t		heredoc_interrupted;
	int					heredoc_mode;
}	t_shell;

//main
t_shell		*prog_data();

t_command	*parser(char *line);
void		append_commands(t_command *new_node);
void		free_commands(t_command *commands);
bool		pipe_checker(char *line);
bool		redirection_checker(char *line);
bool		expansion_checker(char *line);
// void		full_sighandler();
void		sigint_handler(int sig);
bool		we_need_space(char *line);
bool		add_split_words(char ***new_args, int *size, char **split);
bool		add_one_word(char ***new_args, int *size, char *word);
char 		**add_to_array(char **array, int *size, char *str);
bool		is_unquoted_var(const char *arg);
void		append_function(t_command *cmd, int arg_idx, int append);
void		assign_input_fd(t_command *cmd, char *new_in, int new_fd);
int			parser2(char *line, char *new_line, int l);


// static void	*free_split(char **str);
char		*ft_strndup(const char *s1, size_t n);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size); 
int 		strncat_realloc(char **dest, const char *source, size_t n);
char 		*expand_argument(const char *arg, int i, bool in_s, bool in_d);
void 		expansion_trade(void);
void		check_redirs(void);
void		free_double_ptr(char **arr);
void		free_shell_data(t_shell *shell);

//Execution -- | Execution module |
void		ft_execve(t_shell *shell, t_command *cmds);
void		execute_all(t_command *cmds, t_shell *shell);
int			execute_single_command(t_command *cmds, t_shell *shell);
int			execute_pipeline(t_command *cmds, t_shell *shell);
int     	get_heredoc_input(const char *limiter, int expand);
char		*expand_argument_heredoc(const char *arg);

//Exec Helpers
void 		ft_remove_args(t_command *cmd, int start, int count);
void		collect_all_heredocs(void);
void		print_sorted_env(char **envp);
void		apply_redirections(t_command *cmd);
char		*strip_quotes_and_get_delimiter(const char *raw_delim, int *expand);

/* helpers to manage heredoc list on a command */
void		add_heredoc_delim(t_command *cmd, char *delim, int expand);
void		free_heredoc_list(t_command *cmd);

//Builtin
int			execute_builtin(char **args, t_shell *shell);
int			builtin_chkr(char **args);

//Builtin cmds
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(char **args, t_shell *shell);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(char **args, t_shell *shell);
int			builtin_exit(char **args, t_shell *shell);

//Signals
void		signals_interactive(void);
void		signals_execution(void);
void		signals_heredoc(void);
void		sigint_heredoc_handler(int sig);

//Env Manager Utils
int			strcnt(char **envp);

#endif