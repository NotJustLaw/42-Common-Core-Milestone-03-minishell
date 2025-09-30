/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:29:31 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/30 17:32:43 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	quote_expansion(char *line, char *new_line)
{
	int	i;

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

static int	parser39(char **line, char **new_line, int *i, int *l, int flag)
{
	if (flag == 0 && **line == '|')
	{
		**line = '\3';
		l++;
	}
	else if (flag == 0 && (**line == '<' || **line == '>'))
	{	
		*i = quote_expansion(*line, *new_line);
		if (*i < 0)
			return (-1);
		*line += *i;
		*new_line += *i + 2;
		return (1);
	}
	return (0);
}

int parser2(char *line, char *new_line, int l)
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
		else
			if(parser39(&line, &new_line, &i, &l, flag))
				continue ;
		*new_line++ = *line++;
	}
	if (flag == '\'' || flag == '\"')
		return (-1);
	return (l);
}
