/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:50:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/24 10:39:28 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int get_exit_status(void);

char *expand_heredoc(const char *line)
{
	char *result;
	int i;
	int var_len;
	char *var_name;
	char *value;

	result = NULL;
	i = 0;
	var_len = 0;
	var_name = NULL;
	value = NULL;
	while (line[i]) 
	{
		if (line[i] == '$' && line[i+1] && (ft_isalpha(line[i+1]) || line[i+1] == '_'))
		{
			i++;
			int start = i;
			while (ft_isalnum(line[i]) || line[i] == '_')
				i++;
			var_len = i - start;
			var_name = ft_strndup(&line[start], var_len);
			value = getenv(var_name);
			free(var_name);
			if (value)
				strncat_realloc(&result, value, strlen(value));
		} 
		else 
		{
			strncat_realloc(&result, &line[i], 1);
			i++;
		}
	}
	return result;
}

char *expand_argument(const char *arg)
{
	int i = 0;
	bool in_single_quotes = false;
	bool in_double_quotes = false;
	char *result = NULL;

	while (arg[i])
	{
		if (arg[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++;
			continue;
		}
		else if (arg[i] == '\"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++;
			continue;
		}
		else if (arg[i] == '$' && !in_single_quotes)
		{
			i++;
			if (arg[i] && (ft_isalpha(arg[i]) || arg[i] == '_'))
			{
				int start = i;
				while (ft_isalnum(arg[i]) || arg[i] == '_')
					i++;
				int var_len = i - start;
				char *var_name = ft_strndup(&arg[start], var_len);
				if (!var_name)
				{
					free(result);
					return NULL;
				}
				char *value = getenv(var_name);
				free(var_name);
				if (value)
					strncat_realloc(&result, value, strlen(value));
			}
			else
			{
				if (strncat_realloc(&result, "$", 1) == -1)
				{
					free(result);
					return (NULL);
				}
			}
		}
		else
		{
			if (strncat_realloc(&result, &arg[i], 1) == -1)
			{
				free(result);
				return (NULL);
			}
			i++;
		}
	}
	return (result);
}

void expansion_trade(void)
{
	t_command *cmd = prog_data()->commands;
	int	i;

	i = 0;
	while (cmd && cmd->args[i])
	{
		for (int i = 0; cmd->args && cmd->args[i]; i++)
		{
			if (ft_strchr2(cmd->args[i], '$'))
			{
				char *expanded = expand_argument(cmd->args[i]);
				if (!expanded)
					continue;
				free(cmd->args[i]);
				cmd->args[i] = expanded;
			}
		}
		cmd = cmd->next;
	}
}
