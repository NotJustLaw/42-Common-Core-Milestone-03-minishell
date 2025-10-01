/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/10/01 18:39:10 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* append a delimiter to the command's heredoc list (takes ownership of delim) */
void	add_heredoc_delim(t_command *cmd, char *delim, int expand)
{
	t_heredoc	*new;
	t_heredoc	*tmp;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return ;
	new->delim = delim;
	new->expand = expand;
	new->next = NULL;
	if (!cmd->heredocs)
		cmd->heredocs = new;
	else
	{
		tmp = cmd->heredocs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

/* free all heredoc delimiter nodes for a command and clear pointer */
void free_heredoc_list(t_command *cmd)
{
	t_heredoc *it;
	t_heredoc *tmp;

	if (!cmd)
		return;
	it = cmd->heredocs;
	while (it)
	{
		tmp = it->next;
		if (it->delim)
			free(it->delim);
		free(it);
		it = tmp;
	}
	cmd->heredocs = NULL;
}

char *expand_argument_heredoc(const char *arg)
{
    int i = 0;
    char *result = calloc(1, 1);
    if (!result)
        return NULL;

    while (arg[i])
    {
        if (arg[i] == '$')
        {
            i++;
            if (arg[i] == '?')
            {
                char *status_str = ft_itoa(prog_data()->exit_status);
                if (!status_str || strncat_realloc(&result, status_str, strlen(status_str)) == -1)
                    return (free(result), NULL);
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
    return result;
}

