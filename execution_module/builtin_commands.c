/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:05:51 by justlaw           #+#    #+#             */
/*   Updated: 2025/10/01 17:08:09 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

int	builtin_echo(char **args)
{
	int	i;
	int	j;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0' && j > 1)
		{
			n_flag = 1;
			i++;
		}
		else
			break ;
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

int	builtin_cd(char **args, t_shell *shell)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*target_dir;

	old_pwd = getcwd(NULL, 0);
	target_dir = args[1];
	if (!target_dir)
	{
		target_dir = get_env_value(shell->envp, "HOME");
		if (!target_dir || *target_dir == '\0')
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			if (old_pwd) free(old_pwd);
			return (1);
		}
	}
	if (chdir(target_dir) != 0)
	{
		perror(target_dir);
		if (old_pwd) free(old_pwd);
		return (prog_data()->exit_status = 1, 1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		fprintf(stderr, "cd: error retrieving current directory: %s\n", strerror(errno));
		if (old_pwd) free(old_pwd);
		return (prog_data()->exit_status = 1, 1);
	}
	set_env_var(&shell->envp, "OLDPWD", old_pwd ? old_pwd : "");
	set_env_var(&shell->envp, "PWD", new_pwd);
	if (old_pwd) free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_pwd(char **args, t_shell *shell)
{
	char	*pwd;

	(void)args;
	pwd = get_env_value(shell->envp, "PWD");
	if (pwd && *pwd)
		printf("%s\n", pwd);
	else
	{
		char *dir = getcwd(NULL, 0);
		if (!dir)
		{
			fprintf(stderr, "pwd: error retrieving current directory: %s\n", strerror(errno));
			return (1);
		}
		printf("%s\n", dir);
		free(dir);
	}
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
	{
		print_sorted_env(shell->envp);
		return (0);
	}
	while (args[++i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal)
		{
			key = ft_substr(args[i], 0, equal - args[i]);
			value = ft_strdup(equal + 1);
			if (!key || !value)
				return (1);
			set_env_var(&shell->envp, key, value);
			free(key);
			free(value);
		}
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
