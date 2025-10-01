/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:52:02 by notjustlaw        #+#    #+#             */
/*   Updated: 2025/10/01 18:41:58 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

int	get_heredoc_input(const char *limiter, int expand)
{
	int		here_pipe[2];
	char	*line;
	size_t	len;
	int		stdin_copy;
	int		eof_reached;

	if (pipe(here_pipe) < 0)
		return (perror("pipe"), -1);
	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
	{
		close(here_pipe[0]);
		close(here_pipe[1]);
		return (perror("dup"), -1);
	}
	len = ft_strlen(limiter);
	eof_reached = 0;
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			eof_reached = 1;
			break ;
		}
		if (prog_data()->heredoc_interrupted)
		{
			free(line);
			close(here_pipe[0]);
			close(here_pipe[1]);
			dup2(stdin_copy, STDIN_FILENO);
			close(stdin_copy);
			return (-1);
		}
		if ((ft_strncmp(line, limiter, len) == 0) && line[len] == '\0')
		{
			free(line);
			break ;
		}
		if (expand)
		{
			char *orig = line;
			line = expand_argument_heredoc(orig);
			free(orig);
		}
		if (line)
			write(here_pipe[1], line, ft_strlen(line));
		write(here_pipe[1], "\n", 1);
		free(line);
	}
	if (eof_reached && !prog_data()->heredoc_interrupted)
		fprintf(stderr, "minishell: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", limiter);
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	close(here_pipe[1]);
	if (prog_data()->heredoc_interrupted)
	{
		close(here_pipe[0]);
		return (-1);
	}
	return (here_pipe[0]);
}

int	process_heredoc_list_for_cmd(t_command *cmd)
{
	t_heredoc	*h;
	int			last_fd = -1;

	// Single heredoc shortcut
	if ((!cmd->heredocs || cmd->heredocs == NULL) && cmd->heredoc && cmd->delimiter)
	{
		int fd = get_heredoc_input(cmd->delimiter, cmd->heredoc_expand);
		if (fd == -1)
		{
			if (prog_data()->heredoc_interrupted)
				prog_data()->exit_status = 130;
			return (1);
		}
		if (cmd->input_fd > 2)
			close(cmd->input_fd);
		cmd->input_fd = fd;
		return (0);
	}

	if (!cmd->heredocs)
		return (0);

	h = cmd->heredocs;
	while (h)
	{
		int fd = get_heredoc_input(h->delim, h->expand);
		if (fd == -1)
		{
			if (last_fd != -1)
				close(last_fd);
			if (prog_data()->heredoc_interrupted)
				prog_data()->exit_status = 130;
			return (1);
		}
		if (h->next)
			close(fd);      // Immediately close/discard all but last heredoc
		else
			last_fd = fd;   // Only the last one gets assigned
		h = h->next;
	}
	if (last_fd != -1)
	{
		if (cmd->input_fd > 2)
			close(cmd->input_fd);
		cmd->input_fd = last_fd;
	}
	return (0);
}

void	collect_all_heredocs(void)
{
	t_command	*cmd;
	int			ret;

	prog_data()->heredoc_interrupted = 0;
	cmd = prog_data()->commands;
	while (cmd)
	{
		if (cmd->redirection_failed)
		{
			prog_data()->exit_status = 1;
			break ;
		}
		if (cmd->heredoc || cmd->heredocs)
		{
			ret = process_heredoc_list_for_cmd(cmd);
			if (ret == 1)
				break ;
		}
		cmd = cmd->next;
	}
	signals_interactive();
}