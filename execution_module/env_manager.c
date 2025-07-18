/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:56:38 by justlaw           #+#    #+#             */
/*   Updated: 2025/07/17 15:58:57 by justlaw          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Make a deep copy of the system-provided envp so you can manage it yourself
char	**copy_envp(char **envp)
{
	int		lenght;
	char	**copy;
	int		i;

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
	
}
//Set or update key=value in your environment
int	set_env_var(char ***env, const char *key, const char *value)
{
	
}
//Remove the key=value entry from your env list
int	unset_env_var(char ***env, const char *key)
{
	int	i;

	if (!env || !key)
		return (NULL);
	i = 0;
	
	

	return (NULL);
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
