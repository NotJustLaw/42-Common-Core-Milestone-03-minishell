/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:56:38 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/29 21:18:26 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**copy_envp(char **envp)
{
	int		len;
	char	**copy;
	int		i;

	if (!envp)
		return (NULL);
	len = strcnt(envp);
	copy = malloc(sizeof(char *) * (len + 1));
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
		if (ft_strncmp(env[i], key, key_len) == 0
			&& env[i][key_len] == '=')
			return (&env[i][key_len + 1]);
	}
	return (NULL);
}

int	set_env_var(char ***env, const char *key, const char *value)
{
	char	*composed;
	char	*tmp;
	int		count;

	if (!env || !key || !value)
		return (1);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (1);
	composed = ft_strjoin(tmp, value);
	free(tmp);
	if (!composed)
		return (1);
	if (replace_env_var(env, key, composed, ft_strlen(key)))
		return (0);
	count = 0;
	while ((*env)[count])
		count++;
	return (add_env_var(env, composed, count));
}

int	unset_env_var(char ***env, const char *key)
{
	int		i;
	int		j;
	int		key_len;

	if (!env || !*env || !key)
		return (1);
	key_len = ft_strlen(key);
	i = -1;
	while ((*env)[++i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
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

void	free_env(char **env)
{
	int		i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}
