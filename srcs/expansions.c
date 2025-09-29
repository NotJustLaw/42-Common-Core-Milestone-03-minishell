/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:50:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/29 18:12:44 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expand_argument(const char *arg)
{
    int   i = 0;
    bool  in_single_quotes = false;
    bool  in_double_quotes = false;
    char *result = calloc(1, 1);

    if (!result)
        return (NULL);

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
            if (!arg[i] || arg[i] == ' ' || arg[i] == '\t' || arg[i] == '\'' || arg[i] == '\"')
            {
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
                if (strncat_realloc(&result, "$", 1) == -1)
                    return (free(result), NULL);
            }
        }
        else
        {
            if (strncat_realloc(&result, &arg[i], 1) == -1)
                return (free(result), NULL);
            i++;
        }
    }
    return (result);
}

static bool	is_unquoted_var(const char *arg)
{
	if (arg[0] != '$')
		return (false);
	for (int i = 1; arg[i]; i++)
	{
		if (arg[i] == '\'' || arg[i] == '\"')
			return (false);
	}
	return (true);
}

// FIXED: Proper error handling to prevent leaks
static char **add_to_array(char **array, int *size, char *str)
{
    char **new_array;

    new_array = ft_realloc(array, (*size) * sizeof(char *), 
                         (*size + 1) * sizeof(char *));
    if (!new_array)
    {
        free(str);  // Free the string that can't be added
        return (NULL);
    }
    new_array[*size] = str;
    (*size)++;
    return (new_array);
}

// FIXED: Better error handling and cleanup
void expansion_trade(void)
{
    t_command *cmd = prog_data()->commands;

    while (cmd)
    {
        if (!cmd->args)
        {
            cmd = cmd->next;
            continue;
        }

        char **new_args = NULL;
        int new_args_size = 0;
        bool error_occurred = false;

        for (int i = 0; cmd->args[i] && !error_occurred; i++)
        {
            char *expanded = expand_argument(cmd->args[i]);
            if (!expanded) 
            {
                error_occurred = true;
                break;
            }

            if (is_unquoted_var(cmd->args[i]))
            {
                char **split_words = ft_split(expanded, ' ');
                free(expanded);
                if (!split_words) 
                {
                    error_occurred = true;
                    break;
                }

                for (int j = 0; split_words[j]; j++)
                {
                    new_args = add_to_array(new_args, &new_args_size, split_words[j]);
                    if (!new_args)
                    {
                        // Free remaining strings in split_words
                        for (int k = j + 1; split_words[k]; k++)
                            free(split_words[k]);
                        free(split_words);
                        error_occurred = true;
                        break;
                    }
                }
                free(split_words);
            }
            else
            {
                new_args = add_to_array(new_args, &new_args_size, expanded);
                if (!new_args)
                {
                    error_occurred = true;
                    break;
                }
            }
        }

        if (error_occurred)
        {
            // Clean up partial new_args if allocation failed
            if (new_args)
                free_double_ptr(new_args);
        }
        else
        {
            // Success: replace old args with new ones
            free_double_ptr(cmd->args);
            cmd->args = new_args;
            
            // Add NULL terminator
            char **final_args = ft_realloc(new_args, new_args_size * sizeof(char *), 
                                         (new_args_size + 1) * sizeof(char *));
            if (final_args)
            {
                final_args[new_args_size] = NULL;
                cmd->args = final_args;
            }
        }

        // Handle infile and outfile expansion
        if (cmd->infile) 
        {
            char *expanded = expand_argument(cmd->infile);
            if (expanded)
            {
                free(cmd->infile);
                cmd->infile = expanded;
            }
        }
        if (cmd->outfile) 
        {
            char *expanded = expand_argument(cmd->outfile);
            if (expanded)
            {
                free(cmd->outfile);
                cmd->outfile = expanded;
            }
        }

        cmd = cmd->next;
    }
}
