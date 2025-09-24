/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:58:34 by justlaw           #+#    #+#             */
/*   Updated: 2025/09/02 17:29:52 by justlaw          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_MANAGER_H
# define ENV_MANAGER_H

char	**copy_envp(char **envp);
char	*get_env_value(char **env, const char *key);
int		set_env_var(char ***env, const char *key, const char *value);
int		unset_env_var(char ***env, const char *key);
void	free_env(char **env);
int     unset_filter(char *s);

#endif