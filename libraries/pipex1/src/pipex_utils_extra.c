/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:46:06 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/06/05 16:53:32 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = NULL;
}

static char	*handle_slash_cmd(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*search_path_dirs(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*cmd_path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path = envp[i] + 5;
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		cmd_path = join_path(paths[i], cmd);
		if (access(cmd_path, X_OK) == 0)
			return (free_split(paths), cmd_path);
		free(cmd_path);
	}
	free_split(paths);
	return (perror("Not a valid command"), NULL);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	if (ft_strchr(cmd, '/'))
		return (handle_slash_cmd(cmd));
	return (search_path_dirs(cmd, envp));
}
