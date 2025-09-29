/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:05:51 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/29 21:12:10 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;

	i = handle_n_flags(args, &n_flag);
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
	char	*target;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (perror("cd: error retrieving current directory"), 1);
	target = args[1];
	if (!target)
	{
		target = get_env_value(shell->envp, "HOME");
		if (!target || *target == '\0')
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			free(old_pwd);
			return (1);
		}
	}
	if (chdir(target) != 0)
	{
		perror(target);
		free(old_pwd);
		prog_data()->exit_status = 1;
		return (1);
	}
	return (update_pwd(shell, old_pwd));
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
