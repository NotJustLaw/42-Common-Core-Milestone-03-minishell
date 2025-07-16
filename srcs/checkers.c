/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:59:12 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/07/16 17:23:45 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool    pipe_checker(char *line)
{
	int i;
	int found_pipes;

	found_pipes = 0;
	i = 0;
	while (is_wspace(line[i]))
		i++;
	if (line[i] == '|')
		return (false);
	else
	{
		while (line[i])
		{
			if (line[i] == '|')
				found_pipes = 1;
			else if (is_wspace(line[i]) && found_pipes == 1)
				found_pipes = 1;
			else
				found_pipes = 0;
			i++;
		}
	}
	if (found_pipes == 1)
		return (false);
	return (true);
}

bool	redirection_checker(char *line)
{
	int	i;
	int	found_redir;

	found_redir = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '<' || line[i] == '>')
		{
			found_redir = 1;
			if (line[i + 1] == line[i])
				i++;
		}
		else if (is_wspace(line[i]) && found_redir == 1)
			found_redir = 1;
		else if (line[i] == '|' && found_redir == 1)
			return (false);
		else
			found_redir = 0;
		i++;
	}
	if (found_redir == 1)
		return (false);
	return (true);
}

bool	expansion_chekcker(char *line)
{
	int	i;
	bool	in_single_quotes;
	bool	in_double_quotes;
	
	in_single_quotes = false;
	in_double_quotes = false;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++;
		}
		else if (line[i] == '\"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++;
		}
		else if (line[i] == '$' && !in_single_quotes)
		{
			i++;
			if (!line[i])
				return (false);
			if (ft_isalpha(line[i]) || (line[i] == '_' || line[i] == '?'))
			{
				if (line[i] == '?')
				{
					i++;
					continue;
				}
				while (line[i] && !is_wspace(line[i]))
				{
					if (!ft_isalnum(line[i]))
						return(false);
					i++;
				}
			}
			else
				return (false);
		}
		else
			i++;
	}
	return (true);
}
