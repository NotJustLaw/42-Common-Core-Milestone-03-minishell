/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:22:14 by henrique-re       #+#    #+#             */
/*   Updated: 2025/08/07 16:17:49 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	append_function(t_command *cmd, int arg_idx, int append)
{
	if (cmd->args[arg_idx + 1])
	{
		cmd->outfile = ft_strdup(cmd->args[arg_idx + 1]);
		cmd->append = append;
		if (cmd->append)
			cmd->output_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			cmd->output_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
}

static void	here_dock_function(t_command *cmd, int arg_idx, int heredoc)
{
	if (heredoc)
	{
		if (cmd->args[arg_idx + 1])
		{
			cmd->delimiter = ft_strdup(cmd->args[arg_idx + 1]);
			cmd->heredoc = 1;
		}
		else
		{
			if (cmd->args[arg_idx + 1])
			{
				cmd->infile = ft_strdup(cmd->args[arg_idx + 1]);
				cmd->heredoc = 0;
			}
		}
	}
}
void	check_redirs(void)
{
	int			indexes[2];
	bool		quoted;
	t_command	*cmd;

	cmd = prog_data()->commands;
	quoted = false;
	indexes[0] = 0;
	indexes[1] = 0;
	while (prog_data()->commands->args[indexes[0]])
	{
		if (prog_data()->commands->args[indexes[0]][0] == '\"' || prog_data()->commands->args[indexes[0]][0] == '\'')
			quoted = true;
		while (cmd->args[indexes[0]][indexes[1]] && quoted == false)
		{
			if (ft_strncmp(cmd->args[indexes[0]], ">", 1))
				append_function(cmd, indexes[0], indexes[1]);
			else if (ft_strncmp(cmd->args[indexes[0]], ">>", 2))
				append_function(cmd, indexes[0], indexes[1]);
			else if (ft_strncmp(cmd->args[indexes[0]], "<", 1))
				here_dock_function(cmd, indexes[0], indexes[1]);
			else if (ft_strncmp(cmd->args[indexes[0]], "<<", 2))
				here_dock_function(cmd, indexes[0], indexes[1]);
			indexes[1]++;
		}
		indexes[0]++;
	}
}
