/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:14:46 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/29 21:14:05 by skuhlcke         ###   ########.fr       */
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

static void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[j + 1])
		{
			if (ft_strncmp(env[j], env[j + 1], SIZE_MAX) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
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

int	handle_n_flags(char **args, int *n_flag)
{
	int	i;
	int	j;

	i = 1;
	*n_flag = 0;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (j > 1 && args[i][j] == '\0')
		{
			*n_flag = 1;
			i++;
		}
		else
			break ;
	}
	return (i);
}
