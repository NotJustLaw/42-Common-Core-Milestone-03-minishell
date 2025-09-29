/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:13:06 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/29 21:21:19 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_function(t_command *cmd, int arg_idx, int append)
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

char	*strip_quotes_and_get_delimiter(const char *raw, int *expand)
{
	int		quoted;
	char	*out;
	int		i;
	int		j;

	quoted = 0;
	i = 0;
	j = 0;
	out = malloc(ft_strlen(raw) + 1);
	if (!out)
		return (NULL);
	while (raw[i])
	{
		if (raw[i] == '\'' || raw[i] == '"')
		{
			quoted = 1;
			i++;
			continue ;
		}
		out[j++] = raw[i++];
	}
	out[j] = '\0';
	*expand = !quoted;
	return (out);
}

void	assign_input_fd(t_command *cmd, char *new_in, int new_fd)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = new_in;
	if (cmd->input_fd > 2)
		close(cmd->input_fd);
	cmd->input_fd = new_fd;
}
