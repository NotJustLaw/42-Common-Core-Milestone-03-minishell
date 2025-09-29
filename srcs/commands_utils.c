/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 10:19:03 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/29 18:36:24 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_commands(t_command *new_node)
{
	t_command	*current;

	if (!new_node)
		return ;
	if (prog_data()->commands == NULL)
	{
		prog_data()->commands = new_node;
		new_node->next = NULL;
		return ;
	}
	current = prog_data()->commands;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	new_node->next = NULL;
}

static void	free_one_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->input_fd > 2)
		close(cmd->input_fd);
	if (cmd->output_fd > 2)
		close(cmd->output_fd);
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd->delimiter);
	free_heredoc_list(cmd);
	free_double_ptr(cmd->args);
	free(cmd);
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_one_command(current);
		current = next;
	}
}

void	free_double_ptr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_shell_data(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->envp)
	{
		free_double_ptr(shell->envp);
		shell->envp = NULL;
	}
	if (shell->commands)
	{
		free_commands(shell->commands);
		shell->commands = NULL;
	}
	rl_clear_history();
}
