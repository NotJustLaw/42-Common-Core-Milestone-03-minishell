/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:07:09 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/06 16:28:47 by justlaw          ###   ########.fr       */
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
			ft_putstr(shell->envp[i]);
		i++;
	}
	return (0);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	status;

	if (!args[1])
		exit(shell->exit_status);
	if (args[1] && !args[2])
	{
		if (is_numeric(args[1]))
		{
			status = ft_atoi(args[1]);
			exit(status);
		}
		else
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			exit(255);
		}
	}
	if (args[2])
	{
		if (is_numeric(args[1]))
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);
		}
		else
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			exit(255);
		}
	}
	return (0);
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