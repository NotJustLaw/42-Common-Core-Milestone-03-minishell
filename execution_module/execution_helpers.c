/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:31:59 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/29 21:32:00 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*   Created: 2025/09/29 22:38:04 by notjustlaw       #+#    #+#             */
/*   Updated: 2025/09/30 00:12:11 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

static void	child_redirects(t_command *cmds, int in_fd, int pipe_fd[2])
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
}

void	execute_child(t_command *cmds, t_shell *shell,
			int in_fd, int pipe_fd[2])
{
	int	exit_status;

	setup_child_signals();
	child_redirects(cmds, in_fd, pipe_fd);
	apply_redirections(cmds);
	if (builtin_chkr(cmds->args))
	{
		exit_status = execute_builtin(cmds->args, shell);
		free_shell_data(shell);
		exit(exit_status);
	}
	ft_execve(shell, cmds);
	perror("execve");
	free_shell_data(shell);
	exit(127);
}

static void	handle_child_status(int status, t_shell *shell)
{
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
			write(1, "Quit (core dumped)\n", 19);
			shell->exit_status = 131;
		}
	}
}

int	single_command_fork_wait(t_command *cmds, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		execute_child(cmds, shell, STDIN_FILENO, (int [2]){-1, -1});
	if (cmds->input_fd > 0)
		close(cmds->input_fd);
	if (cmds->output_fd > 0)
		close(cmds->output_fd);
	waitpid(pid, &status, 0);
	handle_child_status(status, shell);
	return (0);
}
