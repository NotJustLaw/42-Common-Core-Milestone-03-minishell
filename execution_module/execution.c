/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:54:36 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/07/01 18:08:29 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_execve(t_shell *shell, char *av[])
{
	t_command	*cmds;

	cmds = prog_data()->commands;
	
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

int	builtin_chkr(char *av[])
{
	
}