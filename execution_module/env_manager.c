/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:56:38 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/23 13:59:04 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Make a deep copy of the system-provided envp so you can manage it yourself
char	**copy_envp(char **envp)
{
	int		lenght;
	char	**copy;
	int		i;

	if (!envp)
		return (NULL);
	lenght = strcnt(envp);
	copy = malloc(sizeof(char *) * (lenght + 1));
	if (!copy)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (free_env(copy), NULL);
	}
	copy[i] = NULL;
	return (copy);
}
//Search for key= in your custom env list and return the value part
char	*get_env_value(char **env, const char *key)
{
	int		key_len;
	int		i;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (&env[i][key_len + 1]);
	}
	return (NULL);
}
//Set or update key=value in your environment
int	set_env_var(char ***env, const char *key, const char *value)
{
	int		key_len;
	char	*composed;
	char	*tmp;
	char	**new_env;
	int		i;
	int		j;

	if (!env || !key || !value)
		return (1);
	key_len = ft_strlen(key);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (1);
	composed = ft_strjoin(tmp, value);
	free(tmp);
	if (!composed)
		return (1);
	i = 0;
	while ((*env)[i])
	{
		if (strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = composed;
			return (0);
		}
		i++;
	}
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
	{
		free(composed);
		return (1);
	}
	j = -1;
	while (++j < i)
		new_env[j] = (*env)[j];
	new_env[i] = composed;
	new_env[i + 1] = NULL;
	free(*env);
	(*env) = new_env;
	return (0);
}
//Remove the key=value entry from your env list
int	unset_env_var(char ***env, const char *key)
{
	int	i;
	int	j;
	int	key_len;

	if (!env || !*env || !key)
		return (1);
	i = -1;
	key_len = ft_strlen(key);
	while ((*env)[++i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=')
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j + 1])
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			(*env)[j] = NULL;
			return (0);
		}
	}
	return (1);
}
//Free the memory used by your char **env when Minishell exits 
void	free_env(char **env)
{
	int i;

	i= 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}
