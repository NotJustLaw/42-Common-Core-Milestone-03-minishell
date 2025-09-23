/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:52:02 by notjustlaw        #+#    #+#             */
/*   Updated: 2025/09/23 21:22:32 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int get_heredoc_input(const char *limiter, t_command *cmd)
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
		if (cmd->heredoc_expand)
		{
			char *orig = line;
			line = expand_argument(orig);
			free(orig);
		}
		write(here_pipe[1], line, ft_strlen(line));
		write(here_pipe[1], "\n", 1);
		free(line);
	}
	close(here_pipe[1]);
	return (here_pipe[0]);
}

void	ft_remove_args(t_command *cmd, int start, int count)
{
	int j = 0;
	int k = 0;

	for (j = 0; j < count; j++)
	{
		if (cmd->args[start + j])
		{
			free(cmd->args[start + j]);
			cmd->args[start + j] = NULL;
		}
	}
	j = start + count;
	k = start;
	while (cmd->args[j])
		cmd->args[k++] = cmd->args[j++];
	cmd->args[k] = NULL;
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
			fd = get_heredoc_input(cmd->delimiter, cmd);
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
