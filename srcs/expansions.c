/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:50:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/23 22:28:15 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int get_exit_status(void);

char *expand_argument(const char *arg)
{
    int   i = 0;
    bool  in_single_quotes = false;
    bool  in_double_quotes = false;
    char *result = calloc(1, 1); // start empty

    if (!result)
        return (NULL);

    while (arg[i])
    {
        if (arg[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            i++;
        }
        else if (arg[i] == '\"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            i++;
        }
        else if (arg[i] == '$' && !in_single_quotes)
        {
            i++; // skip $
            if (!arg[i] || arg[i] == ' ' || arg[i] == '\t')
            {
                // lone $
                if (strncat_realloc(&result, "$", 1) == -1)
                    return (free(result), NULL);
            }
            else if (arg[i] == '?')
            {
                char *status_str = ft_itoa(prog_data()->exit_status);
                if (!status_str || strncat_realloc(&result, status_str, strlen(status_str)) == -1)
                {
                    free(status_str);
                    return (free(result), NULL);
                }
                free(status_str);
                i++;
            }
            else if (ft_isalpha(arg[i]) || arg[i] == '_')
            {
                int start = i;
                while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
                    i++;
                int var_len = i - start;
                char *var_name = ft_strndup(&arg[start], var_len);
                if (!var_name)
                    return (free(result), NULL);
                char *value = get_env_value(prog_data()->envp, var_name);
                free(var_name);
                if (value && strncat_realloc(&result, value, strlen(value)) == -1)
                    return (free(result), NULL);
            }
            else
            {
                while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
                    i++;
            }
        }
        else
        {
            if (strncat_realloc(&result, &arg[i], 1) == -1)
                return (free(result), NULL);
            i++;
        }
    }
    return result;
}


void expansion_trade(void)
{
	t_command *cmd = prog_data()->commands;

	while (cmd)
	{
		for (int i = 0; cmd->args && cmd->args[i]; i++)
		{
			char *expanded = expand_argument(cmd->args[i]);
			if (!expanded)
				continue;
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		if (cmd->infile)
		{
			char *expanded = expand_argument(cmd->infile);
			free(cmd->infile);
			cmd->infile = expanded;
		}
		if (cmd->outfile)
		{
			char *expanded = expand_argument(cmd->outfile);
			free(cmd->outfile);
			cmd->outfile = expanded;
		}
		cmd = cmd->next;
	}
}

