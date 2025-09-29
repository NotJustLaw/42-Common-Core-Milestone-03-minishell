/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:29:31 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/29 21:31:35 by henrique-re      ###   ########.fr       */
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

static int	parser2_handle_flag(char *line, char *flag)
{
	if (*flag == 0 && (*line == '\"' || *line == '\''))
	{
		*flag = *line;
		return (1);
	}
	else if (*line == *flag)
	{
		*flag = 0;
		return (1);
	}
	return (0);
}

static int	parser2_handle_token(char **line, char **new_line,
	char *flag, int *l)
{
	int	i;

	if (*flag == 0 && is_wspace(**line))
	{
		*(*new_line)++ = '\2';
		(*line)++;
		return (1);
	}
	else if (*flag == 0 && **line == '|')
	{
		**line = '\3';
		(*l)++;
	}
	else if (*flag == 0 && (**line == '<' || **line == '>'))
	{
		i = quote_expansion(*line, *new_line);
		if (i < 0)
			return (-1);
		*line += i;
		*new_line += i + 2;
		return (1);
	}
	return (0);
}

int	parser2(char *line, char *new_line, int l)
{
	int		i;
	char	flag;

	i = 0;
	flag = 0;
	while (*line)
	{
		if (parser2_handle_flag(line, &flag))
			line++;
		else
		{
			i = parser2_handle_token(&line, &new_line, &flag, &l);
			if (i == 1)
				continue ;
			if (i < 0)
				return (-1);
			*new_line++ = *line++;
		}
	}
	if (flag == '\'' || flag == '\"')
		return (-1);
	return (l);
}
