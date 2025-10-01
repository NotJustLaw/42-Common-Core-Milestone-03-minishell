/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:37 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/10/01 18:38:53 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	gather_heredocs(t_command *cmd, int *indices, int *count)
{
	int		i;
	int		expand;
	char	*stripped;

	i = -1;
	*count = 0;
	while (cmd->args[++i])
	{
		expand = 0;
		if (cmd->args[i + 1] && ft_strncmp(cmd->args[i], "<<", 3) == 0)
		{
			stripped = strip_quotes_and_get_delimiter(cmd->args[i + 1], &expand);
			add_heredoc_delim(cmd, stripped, expand);
			cmd->heredoc = 1;
			indices[*count] = i;
			(*count)++;
		}
	}
}

static void	remove_heredoc_args(t_command *cmd, int *indices, int count)
{
	while (--count >= 0)
		ft_remove_args(cmd, indices[count], 2);
}

static t_command	*new_command(char *str)
{
	t_command	*cmd;
	int			heredoc_indices[256];
	int			heredoc_count;

	cmd = ft_calloc(sizeof(t_command), 1);
	if (!cmd)
		return (NULL);
	cmd->args = ft_split(str, '\2');
	if (!cmd->args)
		return (free(cmd), NULL);
	gather_heredocs(cmd, heredoc_indices, &heredoc_count);
	remove_heredoc_args(cmd, heredoc_indices, heredoc_count);
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
		new_cmd = new_command(cmd_lst[i]);
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
