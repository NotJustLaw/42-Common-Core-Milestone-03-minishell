/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:37 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/22 13:50:29 by notjustlaw       ###   ########.fr       */
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
	else if (*(line - 1) == '<' && *line == '>')
		return (-1);
	else if (*(line - 1) == '>' && *line == '<')
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
	return (cmd);
}


t_command *parser(char *line)
{
	char		*new_line;
	char		**cmd_lst;
	t_command	*new_cmd;
	int			indexes[2];

	new_cmd = NULL;
	new_line = ft_calloc(ft_strlen(line) + 1, 3);
	if (!new_line)
		return (NULL);
	if (!we_need_space(line))
		return (NULL);
	indexes[0] = 0;
	indexes[1] = parser2(line, new_line, 0);
	if (indexes[1] < 0)
		return (NULL);
	cmd_lst = ft_split(new_line, '\3');
	printf("\n\n");
	indexes[0] = 0;
	while (cmd_lst[indexes[0]])
	{
		new_cmd = new_command(cmd_lst[indexes[0]]);
		if (!new_cmd)
			return (NULL);
		append_commands(new_cmd);
		indexes[0]++;
	}
	printf("%d\n", indexes[1]);
	t_command *cmd_iter = prog_data()->commands;
	int i = 0;
	while (cmd_iter && i <= indexes[1])
	{
		int l = 0;
		while (cmd_iter->args && cmd_iter->args[l])
		{
			printf("args on command[%d]:%s\n", l, cmd_iter->args[l]);
			l++;
		}
		printf("\n");
		cmd_iter = cmd_iter->next;
		i++;
	}
	check_redirs();
	t_command *c = prog_data()->commands;
	while (c)
	{
		int j = 0;
		printf("final args: ");
		while (c->args && c->args[j])
			printf("[%s] ", c->args[j++]);
		printf("\n");
		c = c->next;
	}
	expansion_trade();
	t_command *cmd_iter2 = prog_data()->commands;
	i = 0;
	while (cmd_iter2 && i <= indexes[1])
	{
		int l = 0;
		while (cmd_iter2->args && cmd_iter2->args[l])
		{
			printf("args on command[%d]:%s\n", l, cmd_iter2->args[l]);
			l++;
		}
		printf("\n");
		cmd_iter2 = cmd_iter2->next;
		i++;
	}
	free(new_line);
	free_double_ptr(cmd_lst);
	return (NULL);
}
