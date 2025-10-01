/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:04:34 by justlaw           #+#    #+#             */
/*   Updated: 2025/10/01 17:05:23 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(char **args, t_shell *shell)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (builtin_cd(args, shell));
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		return (builtin_echo(args));
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (builtin_pwd(args, shell));
	else if (ft_strncmp(args[0], "export", 7) == 0)
		return (builtin_export(args, shell));
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return (builtin_unset(args, shell));
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return (builtin_env(args, shell));
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		return (builtin_exit(args, shell));
	prog_data()->exit_status = 0;
	return (0);
}

int	builtin_chkr(char **args)
{
	if (!args || !args[0])
		return (0);
	if (!ft_strncmp(args[0], "cd", 3)
		|| !ft_strncmp(args[0], "echo", 5)
		|| !ft_strncmp(args[0], "pwd", 4)
		|| !ft_strncmp(args[0], "export", 7)
		|| !ft_strncmp(args[0], "unset", 6)
		|| !ft_strncmp(args[0], "env", 4)
		|| !ft_strncmp(args[0], "exit", 5))
		return (1);
	return (0);
}