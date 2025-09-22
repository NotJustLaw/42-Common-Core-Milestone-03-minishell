/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:39:04 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/06/05 17:06:49 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	read_ereadoc(int write_fd, const char *limiter)
{
	char	*line;
	size_t	len ;

	len = ft_strlen(limiter);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(line, limiter, len) == 0
			&& line[len] == '\n')
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	close(write_fd);
}

int	open_append_outfile(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}

int	*create_pipes(int num_cmds)
{
	int	count;
	int	*arr;
	int	i;

	count = num_cmds - 1;
	arr = malloc(sizeof(int) * count * 2);
	if (!arr)
	{
		free(arr);
		exit(1);
	}
	i = 0;
	while (i < count)
	{
		if (pipe(arr + 2 * i) < 0)
		{
			perror("pipe error");
			exit(1);
		}
		i++;
	}
	return (arr);
}

void	fork_and_dispatch(t_child_args *args, char **cmds)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < args->num_cmds)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork error");
			exit(1);
		}
		if (pid == 0)
		{
			args->cmd_str = cmds[i];
			if (i == 0)
				first_child(i, args);
			else if (i == args->num_cmds - 1)
				last_child(i, args);
			else
				i_child(i, args);
		}
		i++;
	}
}

void	parent_cleanup(int here_read_fd, int outfile_fd, \
	int *pipes, int num_cmds)
{
	int	i;

	close(here_read_fd);
	close(outfile_fd);
	i = 0;
	while (i < (num_cmds - 1) * 2)
	{
		close(pipes[i]);
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
	free(pipes);
}
