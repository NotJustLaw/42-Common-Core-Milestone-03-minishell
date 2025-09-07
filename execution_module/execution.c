/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:54:36 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/07/08 15:28:15 by justlaw          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_execve(t_shell *shell, t_command *cmds)
{
	char		*path;
	
	if (!cmds || !cmds->args || !cmds->args[0])
		exit(0);
	path = find_cmd_path(cmds->args[0], shell->envp);
	if (!path)
	{
		perror("Command not found");
		exit(127);
	}
	execve(path, cmds->args, shell->envp);
	free(path);
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
