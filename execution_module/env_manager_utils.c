/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:56:15 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/29 21:18:41 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	strcnt(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

void	ft_remove_args(t_command *cmd, int start, int count)
{
	int	j;
	int	k;

	if (!cmd || !cmd->args || start < 0 || count <= 0)
		return ;
	j = 0;
	k = 0;
	while (j < count)
	{
		if (cmd->args[start + j])
		{
			free(cmd->args[start + j]);
			cmd->args[start + j] = NULL;
		}
		j++;
	}
	j = start + count;
	k = start;
	while (cmd->args[j])
		cmd->args[k++] = cmd->args[j++];
	cmd->args[k] = NULL;
}

int	replace_env_var(char ***env, const char *key,
		char *composed, int key_len)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = composed;
			return (1);
		}
		i++;
	}
	return (0);
}

int	add_env_var(char ***env, char *composed, int count)
{
	char	**new_env;
	int		j;

	new_env = malloc((count + 2) * sizeof(char *));
	if (!new_env)
		return (free(composed), 1);
	j = -1;
	while (++j < count)
		new_env[j] = (*env)[j];
	new_env[count] = composed;
	new_env[count + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}
