/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:22:14 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/23 22:42:32 by notjustlaw       ###   ########.fr       */
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
			cmd->output_fd = open(cmd->outfile,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			cmd->output_fd = open(cmd->outfile,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->output_fd < 0)
			perror(cmd->outfile);
	}
}


// static void	append_function(t_command *cmd, int arg_idx, int append)
// {
// 	if (cmd->args[arg_idx + 1])
// 	{
// 		cmd->outfile = ft_strdup(cmd->args[arg_idx + 1]);
// 		cmd->append = append;
// 		if (cmd->append)
// 			cmd->output_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		else
// 			cmd->output_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	}
// }

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
	t_command *cmd = prog_data()->commands;

	while (cmd)
	{
		for (int i = 0; cmd->args && cmd->args[i]; i++)
		{
			if (ft_strncmp(cmd->args[i], ">>", 2) == 0)
			{
				append_function(cmd, i, 1);
				ft_remove_args(cmd, i, 2);
				i--;
			}
			else if (ft_strncmp(cmd->args[i], ">", 1) == 0)
			{
				append_function(cmd, i, 0);
				ft_remove_args(cmd, i, 2);
				i--;
			}
			else if (ft_strncmp(cmd->args[i], "<<", 2) == 0)
			{
				char *raw_delim = cmd->args[i + 1];
				if (raw_delim[0] == '\'' || raw_delim[0] == '"')
				{
					cmd->heredoc_expand = 0;
					cmd->heredoc = 1;
					cmd->delimiter = ft_strndup(raw_delim + 1, strlen(raw_delim) - 2);
				}
				else
				{
					cmd->heredoc_expand = 1;
					cmd->heredoc = 1;
					cmd->delimiter = ft_strdup(raw_delim);
				}
				ft_remove_args(cmd, i, 2);
				i--;
			}
			else if (ft_strncmp(cmd->args[i], "<", 1) == 0)
			{
				if (cmd->args[i + 1])
				{
					cmd->infile = ft_strdup(cmd->args[i + 1]);
					cmd->input_fd = open(cmd->infile, O_RDONLY);
					if (cmd->input_fd < 0)
					{
						perror(cmd->infile);
						prog_data()->exit_status = 1;
						cmd->input_fd = -1;
					}
				}
				ft_remove_args(cmd, i, 2);
				i--;
			}
		}
		cmd = cmd->next;
	}
}
