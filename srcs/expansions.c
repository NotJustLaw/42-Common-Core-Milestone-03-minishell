/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:50:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/01 18:27:24 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int get_exit_status(void);

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
			if (arg[i] == '?')
			{
				char *status_str = ft_itoa(1);// depois adicionar a funcao de ober o exit status
				if (!status_str || strncat_realloc(&result, status_str, strlen(status_str)) == -1)
				{
					free(status_str);
					free(result);
					return NULL;
				}
				free(status_str);
				i++;
			}
			else if (arg[i] && (ft_isalpha(arg[i]) || arg[i] == '_'))
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
	return result;
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
