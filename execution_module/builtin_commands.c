/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:05:51 by justlaw           #+#    #+#             */
/*   Updated: 2025/07/09 16:48:11 by justlaw          ###   ########.fr       */
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
}

int	builtin_pwd(char **args)
{
	char	*dir;

	(void)args;
	dir = getcwd(NULL, 0);
	if (!dir)
	{
		perror("pwd");
		return ;
	}
	printf("%s\n", dir);
	free(dir);
}

int	builtin_export(char **args)
{
	
}

int	builtin_unset(char **args)
{
	
}
