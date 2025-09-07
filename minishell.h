/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:38 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/06 16:27:33 by justlaw          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define MINISHELL_H

# include "libraries/libft/libft.h"
# include "libraries/pipex/src/pipex.h"
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
# include <readline/history.h>
# include <readline/readline.h>
# include <aio.h>
# include <stddef.h>	
# include <sys/time.h>
# include <signal.h>

typedef struct s_command
{
	char				**args;
	char				*dlm;
	int					input_fd;
	int					output_fd;
	struct s_command	*next;

	//Execution
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	char				*delimiter;
}	t_command;

typedef struct s_shell
{	
	char	**envp;
	t_command	*commands;
	int			exit_status;
	int			is_running;
}	t_shell;

//main
t_shell	*prog_data();

t_command	*parser(char *line);
void		append_commands(t_command *new_node);
void		free_commands(t_command *commands);
bool		pipe_checker(char *line);
bool		redirection_checker(char *line);
bool		expansion_chekcker(char *line);
void		full_sighandler();
void		sigint_handler(int sig);
bool		we_need_space(char *line);

// static void	*free_split(char **str);
char	*ft_strndup(const char *s1, size_t n);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int 	strncat_realloc(char **dest, const char *source, size_t n);
char 	*expand_argument(const char *arg);
void 	expansion_trade(void);
void	check_redirs(void);

//Execution -- | Execution module |
void	ft_execve(t_shell *shell, t_command *cmds);
void	execute_all(t_command *cmds, t_shell *shell);
int		execute_single_command(t_command *cmds, t_shell *shell);
int		execute_pipeline(t_command *cmds, t_shell *shell);

//Builtin
int		execute_builtin(char **args, t_shell *shell);
int		builtin_chkr(char **args);

//Builtin cmds
int		builtin_echo(char **args);
int		builtin_cd(char **args);
int		builtin_pwd(char **args);
int		builtin_export(char **args, t_shell *shell);
int		builtin_unset(char **args, t_shell *shell);
int		builtin_env(char **args, t_shell *shell);
int		builtin_exit(char **args, t_shell *shell);

//Env Manager

//Env Manager Utils
int		strcnt(char **envp);

#endif