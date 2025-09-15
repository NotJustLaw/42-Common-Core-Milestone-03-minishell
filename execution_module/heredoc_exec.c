/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:52:02 by notjustlaw        #+#    #+#             */
/*   Updated: 2025/09/16 01:05:57 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int get_heredoc_input(const char *limiter)
{
	int		here_pipe[2];
	char	*line;
	size_t	len;

	if (pipe(here_pipe) < 0)
		return (-1);
	len = ft_strlen(limiter);
	printf("Returning heredoc FD = %d\n", here_pipe[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ((ft_strncmp(line, limiter, len) == 0) && line[len] == '\0'))
		{
			free(line);
			break ;
		}
		write(here_pipe[1], line, ft_strlen(line));
		write(here_pipe[1], "\n", 1);
		free(line);
	}
	close(here_pipe[1]);
	return (here_pipe[0]);
}

void ft_remove_args(t_command *cmd, int start, int count)
{
	int i = start;
	while (cmd->args[i + count])
	{
		cmd->args[i] = cmd->args[i + count];
		i++;
	}
	while (cmd->args[i])
		cmd->args[i++] = NULL;
}

void	collect_all_heredocs(void)
{
	t_command	*cmd;
	int			fd;

	cmd = prog_data()->commands;
	
	while (cmd)
	{
		if (cmd->heredoc && cmd->delimiter)
		{
			fd = get_heredoc_input(cmd->delimiter);
			if (fd == -1)
			{
				perror("heredoc");
				return ;
			}
			cmd->input_fd = fd;
		}
		cmd = cmd->next;
	}
}
