/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:59:12 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/20 18:15:41 by hcarrasq         ###   ########.fr       */
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

bool expansion_chekcker(const char *line)
{
    bool in_squote;
    bool in_dquote;
    size_t i;

	in_squote = false;
	in_dquote = false;
	i = 0;
    while (line[i]) {
        if (line[i] == '\'' && !in_dquote)
		{
            in_squote = !in_squote;
            i++;
        } else if (line[i] == '"' && !in_squote)
		{
            in_dquote = !in_dquote;
            i++;
        } 
		else if (line[i] == '$' && !in_squote) 
		{
            i++;
            if (!line[i]) 
				return false;
            if (line[i] == '?') 
			{
                i++;
                continue;
            }

            if (!(isalpha((unsigned char)line[i]) || line[i] == '_'))
                return false;
            i++;
            while (line[i] && (isalnum((unsigned char)line[i]) || line[i] == '_'))
                i++;
        }
		else
            i++;
    }
    return true;
}