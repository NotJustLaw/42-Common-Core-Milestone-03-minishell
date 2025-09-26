/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:54:36 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/26 14:51:13 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NEW: This function will handle all cleanup within the child process before exiting.
static void cleanup_and_exit_child(int status)
{
    t_shell *shell_in_child = prog_data();
    if (shell_in_child)
    {
        free_shell_data(shell_in_child);
    }
    exit(status);
}

void ft_execve(t_shell *shell, t_command *cmds)
{
	char *path;
	struct stat st;

	if (!cmds || !cmds->args || !cmds->args[0])
		cleanup_and_exit_child(0); // MODIFIED

	path = find_cmd_path(cmds->args[0], shell->envp);
	if (!path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmds->args[0]);
		cleanup_and_exit_child(127); // MODIFIED
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", path);
		free(path);
		cleanup_and_exit_child(126); // MODIFIED
	}
	if (access(path, X_OK) != 0)
	{
		perror("minishell");
		free(path);
		cleanup_and_exit_child(126); // MODIFIED
	}
	if (cmds->input_fd == -1)
	{
		free(path);
    	cleanup_and_exit_child(1); // MODIFIED
	}

	execve(path, cmds->args, shell->envp);

	// This code only runs if execve itself fails.
	perror("minishell");
	free(path);
	cleanup_and_exit_child(127); // MODIFIED
}

void	execute_all(t_command *cmds, t_shell *shell)
{
	if (!cmds)
		return ;
	if (!cmds->next)
		execute_single_command(cmds, shell);
	else
		execute_pipeline(cmds, shell);
}
