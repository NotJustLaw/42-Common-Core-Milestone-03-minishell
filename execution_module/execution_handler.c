/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:08:47 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/26 14:48:31 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_child(t_command *cmds, t_shell *shell, int in_fd, int pipe_fd[2]);

/*
 * Helper: fork a child for a single command and wait for it.
 * Keeps local variable count low.
 */
static int	single_command_fork_wait(t_command *cmds, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		struct sigaction sa;

		sa.sa_handler = SIG_DFL;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
		apply_redirections(cmds);
		if (builtin_chkr(cmds->args))
		{
			int exit_status = execute_builtin(cmds->args, shell);
			free_shell_data(shell);
			exit(exit_status);
		}
		else
			ft_execve(shell, cmds);
		perror("execve");
		free_shell_data(shell);
		exit(127);
	}
	if (cmds->input_fd > 0)
		close(cmds->input_fd);
	if (cmds->output_fd > 0)
		close(cmds->output_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			shell->exit_status = 130;
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			prog_data()->exit_status = 131;
			write(1, "Quit (core dumped)\n", 19);
		}
	}
	return (0);
}

/*
 * Public: execute a single command.
 * This wrapper sets parent signal handling to SIG_IGN while forking/waiting,
 * then restores the previous handlers. Local variable count kept low.
 */
int	execute_single_command(t_command *cmds, t_shell *shell)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	ign;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);

	if (builtin_chkr(cmds->args) && cmds->input_fd <= 0 && cmds->output_fd <= 0)
	{
		/* restore previous signal handlers before returning */
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		return (execute_builtin(cmds->args, shell));
	}
	if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0')
	{
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		shell->exit_status = 0;
		return (0);
	}

	/* Do the fork+wait in a small helper to respect the variable limit. */
	single_command_fork_wait(cmds, shell);

	/* restore the original signal handlers for the interactive shell */
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);

	return (0);
}

/*
 * Helper: create the pipeline (fork children) and wait for them.
 * Params include head so we can close fds later without extra locals.
 */
static int	pipeline_fork_and_manage(t_command *cmds, t_command *head, t_shell *shell)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	int		status;
	pid_t	last_pid;

	in_fd = STDIN_FILENO;
	last_pid = -1;
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
		head = head->next;
	}
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				write(1, "\n", 1);
				shell->exit_status = 130;
			}
			else if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit (core dumped)\n", 19);
		}
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

int	execute_pipeline(t_command *cmds, t_shell *shell)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	ign;
	t_command			*head;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	head = cmds;
	pipeline_fork_and_manage(cmds, head, shell);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	return (0);
}

static void	execute_child(t_command *cmds, t_shell *shell, int in_fd, int pipe_fd[2])
{
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
	{
		int exit_status = execute_builtin(cmds->args, shell);
		free_shell_data(shell);
		exit(exit_status);
	}
	else
		ft_execve(shell, cmds);
	perror("execve");
	free_shell_data(shell);
	exit(127);
}
