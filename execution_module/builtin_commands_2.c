/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:07:09 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/25 19:00:20 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(const char *s);

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	if (args[1])
		return (1);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '=') != NULL)
			ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	status;

	printf("exit\n");
	if (!args[1])
	{
		shell->is_running = 0;
		return (shell->exit_status);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (is_numeric(args[1]))
	{
		status = ft_atoi(args[1]);
		shell->exit_status = (unsigned char)status;
		shell->is_running = 0;
	}
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		shell->exit_status = 255;
		shell->is_running = 0;
	}
	return (shell->exit_status);
}

static int	is_numeric(const char *s)
{
	int i = 0;
	if (s[0] == '-' || s[0] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}