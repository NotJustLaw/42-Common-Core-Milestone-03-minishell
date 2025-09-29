/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:54:36 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/29 21:32:38 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_and_exit_child(int status)
{
	t_shell	*shell_in_child;

	shell_in_child = prog_data();
	if (shell_in_child)
		free_shell_data(shell_in_child);
	exit(status);
}

static char	*validate_exec_path(t_command *cmds, char **envp)
{
	char		*path;
	struct stat	st;

	path = find_cmd_path(cmds->args[0], envp);
	if (!path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmds->args[0]);
		cleanup_and_exit_child(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", path);
		free(path);
		cleanup_and_exit_child(126);
	}
	if (access(path, X_OK) != 0)
	{
		perror("minishell");
		free(path);
		cleanup_and_exit_child(126);
	}
	return (path);
}

void	ft_execve(t_shell *shell, t_command *cmds)
{
	char	*path;

	if (!cmds || !cmds->args || !cmds->args[0])
		cleanup_and_exit_child(0);
	path = validate_exec_path(cmds, shell->envp);
	if (cmds->input_fd == -1)
	{
		free(path);
		cleanup_and_exit_child(1);
	}
	execve(path, cmds->args, shell->envp);
	perror("minishell");
	free(path);
	cleanup_and_exit_child(127);
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
