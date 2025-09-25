/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:14:46 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/25 14:54:54 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_filter(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (-1);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (-1);
		i++;
	}
	return (0);
}

static void sort_env(char **env)
{
	int i, j;
	char *tmp;

	for (i = 0; env[i]; i++)
	{
		for (j = 0; env[j + 1]; j++)
		{
			if (ft_strncmp(env[j], env[j + 1], SIZE_MAX) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
		}
	}
}

void	print_sorted_env(char **envp)
{
	char	**copy;
	char	*eq;
	int		i;
	
	copy = copy_envp(envp);
	sort_env(copy);
	i = -1;
	while (copy[++i])
	{
		eq = ft_strchr(copy[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("declare -x %s=\"%s\"\n", copy[i], eq + 1);
			*eq = '=';
		}
		else
		{
			printf("declare -x %s\n", copy[i]);
		}
	}
	free_double_ptr(copy);
}

void	apply_redirections(t_command *cmd)
{
	if (cmd->input_fd > 2)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_fd > 2)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
}
