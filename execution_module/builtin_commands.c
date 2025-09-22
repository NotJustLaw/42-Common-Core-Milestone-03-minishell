/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:05:51 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/22 13:24:27 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}

int	builtin_cd(char **args)
{
	const char	*path;

	if (!args[1])
		path = getenv("HOME");
	else if (ft_strncmp(args[1], "-", 2) == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
	if (!path)
		return (write(2, "cd: path not set\n", 18), 1);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	return (0);
}

int	builtin_pwd(char **args)
{
	char	*dir;

	(void)args;
	dir = getcwd(NULL, 0);
	if (!dir)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", dir);
	free(dir);
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	if (!args[1])
		return (print_sorted_env(shell->envp), 0);
	while (args[++i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal)
		{
			key = ft_substr(args[i], 0, equal - args[i]);
			value = ft_strdup(equal + 1);
		}
		else
		{
			key = ft_strdup(args[i]);
			value = ft_strdup("");
		}
		if (!key || !value)
			return (1);
		set_env_var(&shell->envp, key, value);
		free(key);
		free(value);
	}
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args[1])
		return (1);
	i = 0;
	status = 0;
	while (args[++i])
	{
		if (unset_filter(args[i]) != -1)
		{
			unset_env_var(&shell->envp, args[i]);
		}
		else
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("`: not a valid identifier\n", 2);
			status = 1;
		}
	}
	return (status);
}
