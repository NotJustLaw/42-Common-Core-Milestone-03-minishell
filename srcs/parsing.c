/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:37 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/10/01 13:42:37 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_command	*new_command(char *str, int i, int expand, char *stripped)
{
	t_command	*cmd;

	cmd = ft_calloc(sizeof(t_command), 1);
	if (!cmd)
		return (NULL);
	cmd->args = ft_split(str, '\2');
	if (!cmd->args)
		return (free(cmd), NULL);
	while (cmd->args[++i])
	{
		if (cmd->args[i + 1] && ft_strncmp(cmd->args[i], "<<", 3) == 0)
		{
			stripped = strip_quotes_and_get_delimiter(cmd->args[i + 1],
					&expand);
			add_heredoc_delim(cmd, stripped);
			cmd->heredoc = 1;
			if (cmd->delimiter)
				free(cmd->delimiter);
			cmd->delimiter = ft_strdup(stripped);
			cmd->heredoc_expand = expand;
			ft_remove_args(cmd, i, 2);
			continue ;
		}
	}
	return (cmd);
}

static char	*prepare_line(char *line)
{
	char	*new_line;

	new_line = ft_calloc(ft_strlen(line) + 1, 3);
	if (!new_line)
		return (NULL);
	if (!we_need_space(line))
		return (free(new_line), NULL);
	if (parser2(line, new_line, 0) < 0)
		return (free(new_line), NULL);
	return (new_line);
}

static bool	build_commands(char **cmd_lst)
{
	int			i;
	t_command	*new_cmd;

	i = 0;
	while (cmd_lst[i])
	{
		new_cmd = new_command(cmd_lst[i], -1, 0, NULL);
		if (!new_cmd)
		{
			free_double_ptr(cmd_lst);
			free_commands(prog_data()->commands);
			prog_data()->commands = NULL;
			return (false);
		}
		append_commands(new_cmd);
		i++;
	}
	return (true);
}

t_command	*parser(char *line)
{
	char		*new_line;
	char		**cmd_lst;

	new_line = prepare_line(line);
	if (!new_line)
		return (NULL);
	cmd_lst = ft_split(new_line, '\3');
	if (!cmd_lst)
		return (free(new_line), NULL);
	if (!build_commands(cmd_lst))
		return (free(new_line), NULL);
	check_redirs();
	expansion_trade();
	free(new_line);
	free_double_ptr(cmd_lst);
	return (prog_data()->commands);
}
