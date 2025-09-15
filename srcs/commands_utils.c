/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 10:19:03 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/07 17:06:20 by notjustlaw       ###   ########.fr       */
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
	t_command	*current;
	t_command	*next;
	int			i;

	if (!commands)
		return ;
	i = 0;
	current = commands;
	next = NULL;
	while (current)
	{
		next = current->next;
		if (current->args)
		{
			while (current->args[i])
			{
				free(current->args[i]);
				i++;
			}
			free(current->args);
		}
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

