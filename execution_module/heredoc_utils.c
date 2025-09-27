/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/27 14:48:11 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* append a delimiter to the command's heredoc list (takes ownership of delim) */
void add_heredoc_delim(t_command *cmd, char *delim)
{
	t_heredoc *node;
	t_heredoc *it;

	if (!cmd || !delim)
	{
		if (delim)
			free(delim);
		return;
	}
	node = malloc(sizeof(t_heredoc));
	if (!node)
	{
		free(delim);
		return;
	}
	node->delim = delim;
	node->next = NULL;
	if (!cmd->heredocs)
	{
		cmd->heredocs = node;
		return;
	}
	it = cmd->heredocs;
	while (it->next)
		it = it->next;
	it->next = node;
}

/* free all heredoc delimiter nodes for a command and clear pointer */
void free_heredoc_list(t_command *cmd)
{
	t_heredoc *it;
	t_heredoc *tmp;

	if (!cmd)
		return;
	it = cmd->heredocs;
	while (it)
	{
		tmp = it->next;
		if (it->delim)
			free(it->delim);
		free(it);
		it = tmp;
	}
	cmd->heredocs = NULL;
}