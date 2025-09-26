/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:52:02 by notjustlaw        #+#    #+#             */
/*   Updated: 2025/09/26 14:30:16 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_heredoc_signals(void)
{
	struct sigaction sa;
	sa.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

int get_heredoc_input(const char *limiter, t_command *cmd)
{
    int     here_pipe[2];
    char    *line;
    size_t  len;
    int     stdin_copy = -1;
    int     eof_reached = 0;

    if (pipe(here_pipe) < 0)
        return (perror("pipe"), -1);
    stdin_copy = dup(STDIN_FILENO);
    len = ft_strlen(limiter);
    setup_heredoc_signals();
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            eof_reached = 1;
            break;
        }

        if (prog_data()->heredoc_interrupted)
        {
            free(line);
            close(here_pipe[0]);
            close(here_pipe[1]);
            
            if (stdin_copy != -1)
            {
                dup2(stdin_copy, STDIN_FILENO);
                close(stdin_copy);
            }
            
            return (-1);
        }
        if ((ft_strncmp(line, limiter, len) == 0) && line[len] == '\0')
        {
            free(line);
            break;
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
    if (eof_reached && !prog_data()->heredoc_interrupted)
        fprintf(stderr, "minishell: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", limiter);
    if (stdin_copy != -1)
    {
        dup2(stdin_copy, STDIN_FILENO);
        close(stdin_copy);
    }
    close(here_pipe[1]);
    if (prog_data()->heredoc_interrupted)
    {
        close(here_pipe[0]);
        return (-1);
    }
    
    return (here_pipe[0]);
}

void	collect_all_heredocs(void)
{
	t_command	*cmd;
	int			fd;
	int			i;
	char		*last_delimiter = NULL;

	prog_data()->heredoc_interrupted = 0;
	cmd = prog_data()->commands;
	while (cmd)
	{
		last_delimiter = NULL;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				if (cmd->args[i + 1] && ft_strncmp(cmd->args[i], "<<", 3) == 0)
				{
					last_delimiter = cmd->args[i + 1];
					cmd->delimiter = last_delimiter;
					cmd->heredoc = 1;
				}
				i++;
			}
		}
		if (cmd->heredoc && cmd->delimiter)
		{
			fd = get_heredoc_input(cmd->delimiter, cmd);
			if (fd == -1)
			{
				if (prog_data()->heredoc_interrupted)
					prog_data()->exit_status = 130;
				break ;
			}
			cmd->input_fd = fd;
		}
		cmd = cmd->next;
	}
	signals_interactive();
}

void	ft_remove_args(t_command *cmd, int start, int count)
{
	int j;
	int k;

	j = 0;
	k = 0;
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