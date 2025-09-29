/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:53:57 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/29 21:13:00 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_pwd(t_shell *shell, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: error retrieving new directory");
		free(old_pwd);
		prog_data()->exit_status = 1;
		return (1);
	}
	set_env_var(&shell->envp, "OLDPWD", old_pwd);
	set_env_var(&shell->envp, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}
