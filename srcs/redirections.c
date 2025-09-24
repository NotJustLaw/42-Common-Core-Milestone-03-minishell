/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:22:14 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/24 14:43:51 by notjustlaw       ###   ########.fr       */
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

static char *strip_quotes_and_get_delimiter(const char *raw_delim, int *expand)
{
	char	*new_str;
	int		i;
	int		j;
	char	quote_char;
	int		has_quotes;

	new_str = malloc(ft_strlen(raw_delim) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	has_quotes = 0;
	while (raw_delim[i])
	{
		if (raw_delim[i] == '\'' || raw_delim[i] == '"')
		{
			has_quotes = 1;
			quote_char = raw_delim[i];
			i++;
			while (raw_delim[i] && raw_delim[i] != quote_char)
				new_str[j++] = raw_delim[i++];
			if (raw_delim[i] == quote_char)
				i++;
		}
		else
			new_str[j++] = raw_delim[i++];
	}
	new_str[j] = '\0';
	*expand = !has_quotes;
	return (new_str);
}

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
				if (cmd->args[i + 1])
				{
					char *raw_delim = cmd->args[i + 1];
					cmd->heredoc = 1;
					cmd->delimiter = strip_quotes_and_get_delimiter(raw_delim, &cmd->heredoc_expand);
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