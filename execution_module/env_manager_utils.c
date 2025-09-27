/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:56:15 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/27 14:53:09 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	strcnt(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

void	ft_remove_args(t_command *cmd, int start, int count)
{
	int	j;
	int	k;

	if (!cmd || !cmd->args || start < 0 || count <= 0)
		return ;
	j = 0;
	k = 0;
	while (j < count)
	{
		if (cmd->args[start + j])
		{
			free(cmd->args[start + j]);
			cmd->args[start + j] = NULL;
		}
		j++;
	}
	j = start + count;
	k = start;
	while (cmd->args[j])
		cmd->args[k++] = cmd->args[j++];
	cmd->args[k] = NULL;
}
