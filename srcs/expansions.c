/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:50:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/07/15 18:29:33 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    expand_argument()
{
    int i;
    int l;
    bool	in_single_quotes;
	bool	in_double_quotes;
    char    *variable_name;
    char    *variable_value;

    variable_name = ft_calloc(1,1);
	in_single_quotes = false;
	in_double_quotes = false;
    l = 0;
    i = 0;
    while (prog_data()->commands->args[l][i])
    {
        if (prog_data()->commands->args[l][i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++;
		}
		else if (prog_data()->commands->args[l][i] == '\'' && !in_single_quotes)
		{
			in_single_quotes = !in_double_quotes;
            i++;
        }
        else if (prog_data()->commands->args[l][i] == '$' && !in_single_quotes)
        {
            i++;
            while (prog_data()->commands->args[l][i] && !is_wspace(prog_data()->commands->args[l][i]))
                *variable_name++ = prog_data()->commands->args[l][i++];
            variable_value = getenv(variable_name);
        }
    }
}

// You need to provide your own ft_itoa or equivalent
extern char *ft_itoa(int n);

// Use this to get your shell exit status; change as needed
extern int get_exit_status(void);

char *expand_argument(const char *arg)
{
    int i = 0;
    bool in_single_quotes = false;
    bool in_double_quotes = false;
    char *result = calloc(1, 1); // empty string

    while (arg[i])
    {
        if (arg[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            strncat_realloc(&result, &arg[i], 1);
            i++;
        }
        else if (arg[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            strncat_realloc(&result, &arg[i], 1);
            i++;
        }
        else if (arg[i] == '$' && !in_single_quotes)
        {
            i++;
            if (arg[i] == '?') // special variable $?
            {
                char *status_str = ft_itoa(get_exit_status());
                if (!status_str)
                {
                    free(result);
                    return NULL;
                }
                strncat_realloc(&result, status_str, strlen(status_str));
                free(status_str);
                i++;
            }
            else if (isalpha(arg[i]) || arg[i] == '_')
            {
                int start = i;
                while (isalnum(arg[i]) || arg[i] == '_')
                    i++;
                int var_len = i - start;
                char *var_name = strndup(&arg[start], var_len);
                if (!var_name)
                {
                    free(result);
                    return NULL;
                }
                char *value = getenv(var_name);
                free(var_name);
                if (value)
                    strncat_realloc(&result, value, strlen(value));
                // if value == NULL, just skip (empty string)
            }
            else
            {
                // Just treat $ literally if no valid var name
                strncat_realloc(&result, "$", 1);
            }
        }
        else
        {
            strncat_realloc(&result, &arg[i], 1);
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
                continue;  // handle error as you want
            free(cmd->args[i]);
            cmd->args[i] = expanded;
        }
        cmd = cmd->next;
    }
}