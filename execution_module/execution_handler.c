/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:08:47 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/25 14:37:15 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_child(t_command *cmds, t_shell *shell, int in_fd, int pipe_fd[2]);

int	execute_single_command(t_command *cmds, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (builtin_chkr(cmds->args) && cmds->input_fd <= 0 && cmds->output_fd <= 0)
		return (execute_builtin(cmds->args, shell));
	if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0')
	{
		shell->exit_status = 0;
		return (0);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		apply_redirections(cmds);
		if (builtin_chkr(cmds->args))
			exit(execute_builtin(cmds->args, shell));
		else
			ft_execve(shell, cmds);
		perror("execve");
		exit(127);
	}
	// Parent closes its copy of the redirection file descriptors.
	if (cmds->input_fd > 0)
		close(cmds->input_fd);
	if (cmds->output_fd > 0)
		close(cmds->output_fd);
	// REMOVED: if (cmds->heredoc > 0) close(cmds->heredoc); <-- THIS WAS THE BUG

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (0);
}

int	execute_pipeline(t_command *cmds, t_shell *shell)
{
	int			pipe_fd[2];
	int			in_fd;
	pid_t		pid;
	int			status;
	pid_t		last_pid;
	t_command	*head;

	in_fd = STDIN_FILENO;
	last_pid = -1;
	head = cmds;
	while (cmds)
	{
		if (cmds->next)
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
			execute_child(cmds, shell, in_fd, pipe_fd);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmds->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		last_pid = pid;
		cmds = cmds->next;
	}
	while (head)
	{
		if (head->input_fd > 0)
			close(head->input_fd);
		if (head->output_fd > 0)
			close(head->output_fd);
		// REMOVED: if (head->heredoc > 0) close(head->heredoc); <-- THIS WAS THE BUG
		head = head->next;
	}
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

static void	execute_child(t_command *cmds, t_shell *shell, int in_fd, int pipe_fd[2])
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmds->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	apply_redirections(cmds);
	if (builtin_chkr(cmds->args))
		exit(execute_builtin(cmds->args, shell));
	else
		ft_execve(shell, cmds);
	perror("execve");
	exit(127);
}