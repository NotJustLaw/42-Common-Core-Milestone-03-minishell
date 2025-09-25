/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:52:02 by notjustlaw        #+#    #+#             */
/*   Updated: 2025/09/25 18:03:12 by notjustlaw       ###   ########.fr       */
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

    if (pipe(here_pipe) < 0)
        return (perror("pipe"), -1);
    
    // Make a copy of stdin for restoration later
    stdin_copy = dup(STDIN_FILENO);
    
    len = ft_strlen(limiter);

    // Arm the special heredoc handler
    setup_heredoc_signals();
    
    while (1)
    {
        line = readline("> ");
        
        // Check if readline returned NULL due to our signal handler
        // closing stdin or a normal EOF (CTRL+D)
        if (!line || prog_data()->heredoc_interrupted)
        {
            if (line) 
                free(line);
                
            close(here_pipe[0]);
            close(here_pipe[1]);
            
            // Restore stdin for future commands
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
        
        // Your expansion and write logic...
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
    
    // Restore stdin if we haven't already
    if (stdin_copy != -1)
    {
        dup2(stdin_copy, STDIN_FILENO);
        close(stdin_copy);
    }
    
    close(here_pipe[1]);
    return (here_pipe[0]);
}

void	collect_all_heredocs(void)
{
	t_command	*cmd;
	int			fd;

	// Reset the flag before we begin.
	prog_data()->heredoc_interrupted = 0;
	cmd = prog_data()->commands;
	while (cmd)
	{
		if (cmd->heredoc && cmd->delimiter)
		{
			fd = get_heredoc_input(cmd->delimiter, cmd);
			if (fd == -1)
			{
				if (prog_data()->heredoc_interrupted)
					prog_data()->exit_status = 130;
				break; // Stop processing more heredocs
			}
			cmd->input_fd = fd;
		}
		cmd = cmd->next;
	}
	// IMPORTANT: Restore the main interactive signals for the next prompt.
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