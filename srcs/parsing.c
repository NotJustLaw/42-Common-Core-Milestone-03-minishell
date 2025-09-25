/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:37 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/25 20:28:19 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	quote_expansion(char *line, char *new_line)
{
	int i;

	i = 1;
	*new_line++ = '\2';
	*new_line++ = *line++;
	if (*(line - 1) == *line)
	{	
		*new_line++ = *line++;
		i++;
	}
	else if ((*(line - 1) == '<' && *line == '>')
		|| (*(line - 1) == '>' && *line == '<'))
		return (-1);
	if (*(line) == '>' || *(line) == '<')
		return (-1);
	*new_line++ = '\2';
	return (i);
}

static int parser2(char *line, char *new_line, int l)
{
	int	i;
	char flag;

	i = 0;
	flag = 0;
	while (*line)
	{
		if (flag == 0 && (*line == '\"' || *line == '\''))
			flag = *line ;
		else if (*line == flag)
			flag = 0;
		else if (flag == 0 && is_wspace(*line))
		{
			*new_line++ = '\2';
			line++;
			continue;
		}
		else if (flag == 0 && *line == '|')
		{
			*line = '\3';
			l++;
		}
		else if (flag == 0 && (*line == '<' || *line == '>'))
		{	
			i = quote_expansion(line, new_line);
			if (i < 0)
				return (-1);
			line += i;
			new_line += i + 2;
			continue;
		}
		*new_line++ = *line++;
	}
	if (flag == '\'' || flag == '\"')
		return (-1);
	return (l);
}

static t_command *new_command(char *str)
{
	t_command	*cmd;

	cmd = ft_calloc(sizeof(t_command), 1);
	if (!cmd)
		return (NULL);
	cmd->args = ft_split(str, '\2');
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

t_command *parser(char *line)
{
	char		*new_line;
	char		**cmd_lst;
	t_command	*new_cmd;
	int			i;

	new_cmd = NULL;
	new_line = ft_calloc(ft_strlen(line) + 1, 3);
	if (!new_line)
		return (NULL);
	if (!we_need_space(line))
	{
		free(new_line);
		return (NULL);
	}
	if (parser2(line, new_line, 0) < 0)
	{
		free(new_line);
		return (NULL);
	}
	cmd_lst = ft_split(new_line, '\3');
	if (!cmd_lst)
	{
		free(new_line);
		return (NULL);
	}
	i = 0;
	while (cmd_lst[i])
	{
		new_cmd = new_command(cmd_lst[i]);
		if (!new_cmd)
		{
			free(new_line);
			free_double_ptr(cmd_lst);
			free_commands(prog_data()->commands);
			prog_data()->commands = NULL;
			return (NULL);
		}
		append_commands(new_cmd);
		i++;
	}
	check_redirs();
	expansion_trade();
	free(new_line);
	free_double_ptr(cmd_lst);
	return (prog_data()->commands);
}