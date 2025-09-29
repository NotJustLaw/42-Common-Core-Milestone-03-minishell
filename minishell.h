/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:38 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/29 21:30:23 by skuhlcke         ###   ########.fr       */
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

/* small linked list to hold multiple heredoc delimiters per command */
typedef struct s_heredoc
{
	char				*delim;
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

	/* new: list of heredoc delimiters (left-to-right order) */
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

// static void	*free_split(char **str);
char		*ft_strndup(const char *s1, size_t n);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size); 
int 		strncat_realloc(char **dest, const char *source, size_t n);
char 		*expand_argument(const char *arg);
void 		expansion_trade(void);
void		check_redirs(void);
void		free_double_ptr(char **arr);
void		free_shell_data(t_shell *shell);

//Execution -- | Execution module |
void		ft_execve(t_shell *shell, t_command *cmds);
void		execute_all(t_command *cmds, t_shell *shell);
int			execute_single_command(t_command *cmds, t_shell *shell);
int			execute_pipeline(t_command *cmds, t_shell *shell);
int			get_heredoc_input(const char *limiter, t_command *cmd);
char		*expand_argument_heredoc(const char *arg);
void		execute_child(t_command *cmds, t_shell *shell,
			int in_fd, int pipe_fd[2]);

//Exec Helpers
void 		ft_remove_args(t_command *cmd, int start, int count);
void		collect_all_heredocs(void);
void		print_sorted_env(char **envp);
void		apply_redirections(t_command *cmd);
char		*strip_quotes_and_get_delimiter(const char *raw_delim, int *expand);
int			handle_n_flags(char **args, int *n_flag);
int			single_command_fork_wait(t_command *cmds, t_shell *shell);
int			pipeline_fork_and_manage(t_command *cmds, t_shell *shell);


/* helpers to manage heredoc list on a command */
void		add_heredoc_delim(t_command *cmd, char *delim);
void		free_heredoc_list(t_command *cmd);

//Builtin
int			execute_builtin(char **args, t_shell *shell);
int			builtin_chkr(char **args);
int			update_pwd(t_shell *shell, char *old_pwd);

//Builtin cmds
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(char **args);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(char **args, t_shell *shell);
int			builtin_exit(char **args, t_shell *shell);
int			replace_env_var(char ***env, const char *key,
		char *composed, int key_len);
int			add_env_var(char ***env, char *composed, int count);

//Signals
void		signals_interactive(void);
void		signals_execution(void);
void		signals_heredoc(void);
void		sigint_heredoc_handler(int sig);

//Env Manager Utils
int			strcnt(char **envp);

#endif