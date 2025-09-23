/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:54:36 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/23 22:46:00 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_execve(t_shell *shell, t_command *cmds)
{
	char *path;
	struct stat st;

	if (!cmds || !cmds->args || !cmds->args[0])
		exit(0);
	path = find_cmd_path(cmds->args[0], shell->envp);
	if (!path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmds->args[0]);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", path);
		free(path);
		exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		perror("minishell");
		free(path);
		exit(126);
	}
	if (cmds->input_fd == -1)
	{
		free(path);
    	exit(1);
	}
	execve(path, cmds->args, shell->envp);
	perror("minishell");
	free(path);
	exit(127);
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
