/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:22:14 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/15 23:28:43 by notjustlaw       ###   ########.fr       */
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

// static void	here_dock_function(t_command *cmd, int arg_idx, int heredoc)
// {
//     if (cmd->args[arg_idx + 1])
//     {
//         if (heredoc)
//         {
//             cmd->delimiter = ft_strdup(cmd->args[arg_idx + 1]);
//             cmd->heredoc = 1;
//         }
//         else
//         {
//             cmd->infile = ft_strdup(cmd->args[arg_idx + 1]);
//             cmd->heredoc = 0;
//         }
//     }
// }
void check_redirs(void)
{
	t_command *cmd;
	
	cmd = prog_data()->commands;
	while (cmd)
	{
		int i = 0;

		while (cmd->args && cmd->args[i])
		{
			if (ft_strncmp(cmd->args[i], ">>", 2) == 0)
				append_function(cmd, i, 1);
			else if (ft_strncmp(cmd->args[i], ">", 1) == 0)
				append_function(cmd, i, 0);
			else if (ft_strncmp(cmd->args[i], "<<", 2) == 0)
			{
				if (cmd->args[i + 1])
				{
					cmd->delimiter = ft_strdup(cmd->args[i + 1]);
					cmd->heredoc = 1;
					printf("HEREDOC DETECTED: delimiter=%s\n", cmd->delimiter);
				}
				ft_remove_args(cmd, i, 2);
				i--;
			}
			else if (ft_strncmp(cmd->args[i], "<", 1) == 0)
			{
				if (cmd->args[i + 1])
					cmd->infile = ft_strdup(cmd->args[i + 1]);
				ft_remove_args(cmd, i, 2);
				i--;
			}
			i++;
		}
		cmd = cmd->next;
	}
}

