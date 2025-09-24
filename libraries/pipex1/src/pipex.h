/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:00:26 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/06 13:55:42 by justlaw          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../../libft/libft.h"
# include "get_next_line_bonus.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_fd_list
{
	int		fd_in;
	int		fd_out;
	int		pipefd[2];
	char	*cmd;
	char	**envp;
}		t_fd_list;

typedef struct s_child_args
{
	int		here_fd;
	int		*pipes;
	int		num_cmds;
	int		outfile_fd;
	char	*cmd_str;
	char	**envp;
}		t_child_args;

//Main functions
void	handle_two_command_mode(int ac, char *av[], char **envp);

//FD-s handlers
int		fd_in_handler(char *av[]);
int		fd_out_handler(char *av[]);
int		pipe_generator(int pipefd[2]);

//Forkers
void	forking_process(t_fd_list *fds, char *av[], char **envp);
void	child1(t_fd_list *fds, char *av[], char **envp);
void	child2(t_fd_list *fds, char *av[], char **envp);

//Error handlers
int		error_handler(int ac, char *av[]);
void	print_error(void);

//Helpers
void	free_split(char **arr);
void	free_split_and_exit(char **arr, int exit_code);
char	*join_path(const char *dir, const char *cmd);
char	**ft_parser(char av[]);
char	**intelligent_parser(char av[], t_child_args *args);
char	*find_cmd_path(char *cmd, char **envp);

//Bonus related
void	handle_here_doc_mode(int ac, char *av[], char **envp);
void	handle_multi_pipe_mode(int ac, char *av[], char **envp);

//Child related
void	first_child(int idx, t_child_args *args);
void	i_child(int idx, t_child_args *args);
void	last_child(int idx, t_child_args *args);

//Handle Document utils
void	read_ereadoc(int write_fd, const char *limiter);
int		open_append_outfile(const char *filename);
int		*create_pipes(int num_cmds);
void	fork_and_dispatch(t_child_args *args, char **cmds);
void	parent_cleanup(int here_read_fd, int outfile_fd, \
			int *pipes, int num_cmds);

#endif