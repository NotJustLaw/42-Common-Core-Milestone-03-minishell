/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:52:02 by notjustlaw        #+#    #+#             */
/*   Updated: 2025/09/29 22:08:07 by skuhlcke         ###   ########.fr       */
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

static int init_heredoc_resources(int here_pipe[2], int *stdin_copy)
{
	if (pipe(here_pipe) < 0)
		return (perror("pipe"), -1);
	*stdin_copy = dup(STDIN_FILENO);
	if (*stdin_copy == -1)
	{
		close(here_pipe[0]);
		close(here_pipe[1]);
		return (perror("dup"), -1);
	}
	return (0);
}

static int is_limiter_line(const char *line, const char *limiter)
{
	size_t len;

	if (!line || !limiter)
		return (0);
	len = ft_strlen(limiter);
	return (ft_strncmp(line, limiter, len) == 0 && line[len] == '\0');
}

static void write_heredoc_line(int fd, char *line, t_command *cmd)
{
	if (cmd && cmd->heredoc_expand)
	{
		char *orig = line;
		line = expand_argument_heredoc(orig);
		free(orig);
	}
	if (line)
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

static int read_heredoc_loop(int here_pipe[2], const char *limiter,
							 t_command *cmd, int stdin_copy)
{
	char *line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (1);
		if (prog_data()->heredoc_interrupted)
		{
			free(line);
			close(here_pipe[0]);
			close(here_pipe[1]);
			dup2(stdin_copy, STDIN_FILENO);
			close(stdin_copy);
			return (-1);
		}
		if (is_limiter_line(line, limiter))
		{
			free(line);
			return (0);
		}
		write_heredoc_line(here_pipe[1], line, cmd);
	}
}

int get_heredoc_input(const char *limiter, t_command *cmd)
{
	int here_pipe[2];
	int stdin_copy;
	int status;

	if (init_heredoc_resources(here_pipe, &stdin_copy) < 0)
		return (-1);
	setup_heredoc_signals();
	status = read_heredoc_loop(here_pipe, limiter, cmd, stdin_copy);
	if (status == -1)
		return (-1);
	if (status == 1 && !prog_data()->heredoc_interrupted)
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

static int	handle_single_heredoc(t_command *cmd)
{
	int	fd;

	fd = get_heredoc_input(cmd->delimiter, cmd);
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

static int	process_heredoc_list(t_heredoc *h, t_command *cmd, int fd, int kept_fd)
{
	while (h)
	{
		fd = get_heredoc_input(h->delim, cmd);
		if (fd == -1)
		{
			if (kept_fd != -1)
				close(kept_fd);
			if (prog_data()->heredoc_interrupted)
				prog_data()->exit_status = 130;
			return (1);
		}
		if (kept_fd != -1)
			close(kept_fd);
		kept_fd = fd;
		h = h->next;
	}
	if (kept_fd != -1)
	{
		if (cmd->input_fd > 2)
			close(cmd->input_fd);
		cmd->input_fd = kept_fd;
	}
	return (0);
}

int	process_heredoc_list_for_cmd(t_command *cmd)
{
	if ((!cmd->heredocs || cmd->heredocs == NULL) && cmd->heredoc && cmd->delimiter)
		return (handle_single_heredoc(cmd));
	if (!cmd->heredocs)
		return (0);
	return (process_heredoc_list(cmd->heredocs, cmd, 0, -1));
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
