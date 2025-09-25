/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 10:19:03 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/25 19:15:57 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_commands(t_command *new_node)
{
	t_command *current;

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

void	free_commands(t_command *commands)
{
	t_command *current;
	t_command *next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_double_ptr(current->args); // Use your existing helper function
		free(current->infile);
		free(current->outfile);
		free(current->delimiter);
		free(current);
		current = next;
	}
}

void	free_double_ptr(char **arr)
{
	int	i = 0;

	if (!arr)
		return;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_shell_data(t_shell *shell)
{
	if (!shell)
		return;
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
