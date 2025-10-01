/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:22:14 by henrique-re       #+#    #+#             */
/*   Updated: 2025/10/01 16:55:07 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_output(t_command *cmd, int *i, int append)
{
	append_function(cmd, *i, append);
	if (cmd->output_fd < 0)
		cmd->redirection_failed = 1;
	ft_remove_args(cmd, *i, 2);
	if (*i > 0)
		(*i)--;
}

static void	handle_heredoc(t_command *cmd, int *i)
{
	char	*raw;

	if (cmd->args[*i + 1])
	{
		if (cmd->delimiter)
			free(cmd->delimiter);
		raw = cmd->args[*i + 1];
		cmd->heredoc = 1;
		cmd->delimiter = strip_quotes_and_get_delimiter(raw,
				&cmd->heredoc_expand);
	}
	ft_remove_args(cmd, *i, 2);
	if (*i > 0)
		(*i)--;
}

static void	handle_input(t_command *cmd, int *i)
{
	int		new_fd;
	char	*new_in;

	if (!cmd->args[*i + 1])
	{
		ft_remove_args(cmd, *i, 2);
		if (*i > 0)
			(*i)--;
		return ;
	}
	new_in = ft_strdup(cmd->args[*i + 1]);
	new_fd = open(new_in, O_RDONLY);
	if (new_fd < 0)
	{
		perror(new_in);
		cmd->redirection_failed = 1;
		prog_data()->exit_status = 1;
		free(new_in);
	}
	else
		assign_input_fd(cmd, new_in, new_fd);
	ft_remove_args(cmd, *i, 2);
	if (*i > 0)
		(*i)--;
}

static void	scan_redirs(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (!ft_strncmp(cmd->args[i], ">>", 2))
			handle_output(cmd, &i, 1);
		else if (!ft_strncmp(cmd->args[i], ">", 1))
			handle_output(cmd, &i, 0);
		else if (!ft_strncmp(cmd->args[i], "<<", 2))
			handle_heredoc(cmd, &i);
		else if (!ft_strncmp(cmd->args[i], "<", 1))
			handle_input(cmd, &i);
		else
			i++;
	}
}

void	check_redirs(void)
{
	t_command	*cmd;

	cmd = prog_data()->commands;
	while (cmd)
	{
		cmd->redirection_failed = 0;
		scan_redirs(cmd);
		cmd = cmd->next;
	}
}
