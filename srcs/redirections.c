/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:22:14 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/29 17:39:13 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	append_function(t_command *cmd, int arg_idx, int append)
{
	if (cmd->args[arg_idx + 1])
	{
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->output_fd > 2)
			close(cmd->output_fd);
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

char *strip_quotes_and_get_delimiter(const char *raw_delim, int *expand)
{
    char    *new_str;
    int     i = 0;
    int     j = 0;
    bool    has_quotes = false;
    char    quote_char = 0;

    for (int k = 0; raw_delim[k]; k++)
    {
        if (raw_delim[k] == '\'' || raw_delim[k] == '"')
        {
            has_quotes = true;
            break;
        }
    }
    *expand = !has_quotes;
    new_str = malloc(ft_strlen(raw_delim) + 1);
    if (!new_str)
        return (NULL);
    while (raw_delim[i])
    {
        if ((raw_delim[i] == '\'' || raw_delim[i] == '"') && quote_char == 0)
            quote_char = raw_delim[i];
        else if (raw_delim[i] == quote_char)
            quote_char = 0;
        else
            new_str[j++] = raw_delim[i];
        i++;
    }
    new_str[j] = '\0';
    return (new_str);
}

void check_redirs(void)
{
	t_command *cmd = prog_data()->commands;

	while (cmd)
	{
		dprintf(2, ">>> ENTER check_redirs()\n");
		cmd->redirection_failed = 0;
		for (int i = 0; cmd->args && cmd->args[i]; i++)
		{
			if (ft_strncmp(cmd->args[i], ">>", 2) == 0)
			{
				append_function(cmd, i, 1);
				if (cmd->output_fd < 0)
					cmd->redirection_failed = 1;
				ft_remove_args(cmd, i, 2);
				i--;
			}
			else if (ft_strncmp(cmd->args[i], ">", 1) == 0)
			{
				append_function(cmd, i, 0);
				if (cmd->output_fd < 0)
					cmd->redirection_failed = 1;
				ft_remove_args(cmd, i, 2);
				i--;
			}
			else if (ft_strncmp(cmd->args[i], "<<", 2) == 0)
			{
				if (cmd->args[i + 1])
				{
					if (cmd->delimiter)
						free(cmd->delimiter);
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
					int new_fd;
					char *new_infile;

					/* prepare filename and open it first */
					if (cmd->infile)
						free(cmd->infile);
					new_infile = ft_strdup(cmd->args[i + 1]);
					new_fd = open(new_infile, O_RDONLY);
					if (new_fd < 0)
					{
						/* failed to open new infile: restore state, mark error */
						perror(new_infile);
						cmd->redirection_failed = 1;
						prog_data()->exit_status = 1;
						free(new_infile);
						/* do not change cmd->input_fd */
					}
					else
					{
						/* success: close previous non-std input fd, assign new */
						if (cmd->infile)
							free(cmd->infile);
						cmd->infile = new_infile;
						if (cmd->input_fd > 2)
							close(cmd->input_fd);
						cmd->input_fd = new_fd;
					}
				}
				ft_remove_args(cmd, i, 2);
				i--;
			}
		}
		cmd = cmd->next;
	}
}