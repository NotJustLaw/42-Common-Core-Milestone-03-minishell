/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:08:47 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/16 01:15:14 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_child(t_command *cmds, t_shell *shell, int in_fd, int pipe_fd[2]);

int execute_single_command(t_command *cmds, t_shell *shell)
{
    pid_t pid;

    if (builtin_chkr(cmds->args))
        return (execute_builtin(cmds->args, shell));
    else
    {
        pid = fork();
        if (pid == 0)
        {
            if (cmds->input_fd > 0)
            {
                dup2(cmds->input_fd, STDIN_FILENO);
                close(cmds->input_fd);
            }
            if (cmds->output_fd > 0)
            {
                dup2(cmds->output_fd, STDOUT_FILENO);
                close(cmds->output_fd);
            }
            printf("Running single command: %s\n", cmds->args[0]);
            ft_execve(shell, cmds);
            perror("execve");
            exit(127);
        }
        else
            wait(NULL);
    }
    return (0);
}


int	execute_pipeline(t_command *cmds, t_shell *shell)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	int		status;
	int		last_pid;

	in_fd = 0;
	while (cmds)
	{
		if (pipe_generator(pipe_fd))
			return (perror("pipe"), -1);
		pid = fork();
		if (pid == 0)
			execute_child(cmds, shell, in_fd, pipe_fd);
		else
		{
			close(pipe_fd[1]);
			if (in_fd != 0)
				close(in_fd);
			in_fd = pipe_fd[0];
			cmds = cmds->next;
		}
		last_pid = pid;
	}
	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
			shell->exit_status = WEXITSTATUS(status);
	}
	return (0);
}

static void	execute_child(t_command *cmds, t_shell *shell, int in_fd, int pipe_fd[2])
{
	dup2(in_fd, STDIN_FILENO);
	if (cmds->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(in_fd);
	if (cmds->input_fd > 0)
	{
		dup2(cmds->input_fd, STDIN_FILENO);
		close(cmds->input_fd);
	}
	else if (in_fd != STDIN_FILENO)
		dup2(in_fd, STDIN_FILENO);
	printf("Running command: %s\n", cmds->args[0]);
	if (builtin_chkr(cmds->args))
		exit(execute_builtin(cmds->args, shell));
	else
		ft_execve(shell, cmds);
	perror("execve");
	exit(127);
}

