/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/29 21:24:34 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* append a delimiter to the command's heredoc list (takes ownership of delim) */
void add_heredoc_delim(t_command *cmd, char *delim)
{
	t_heredoc *node;
	t_heredoc *it;

	if (!cmd || !delim)
	{
		if (delim)
			free(delim);
		return;
	}
	node = malloc(sizeof(t_heredoc));
	if (!node)
	{
		free(delim);
		return;
	}
	node->delim = delim;
	node->next = NULL;
	if (!cmd->heredocs)
	{
		cmd->heredocs = node;
		return;
	}
	it = cmd->heredocs;
	while (it->next)
		it = it->next;
	it->next = node;
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

static int	run_pipeline_cmd(t_command *cmds, t_shell *shell,
			int *in_fd, pid_t *last_pid)
{
	int	pipe_fd[2];
	pid_t	pid;

	if (cmds->next && pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		execute_child(cmds, shell, *in_fd, pipe_fd);
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmds->next)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
	*last_pid = pid;
	return (0);
}

int	pipeline_fork_and_manage(t_command *cmds, t_shell *shell)
{
	int		in_fd;
	int		status;
	pid_t	last_pid;

	in_fd = STDIN_FILENO;
	last_pid = -1;
	while (cmds)
	{
		if (run_pipeline_cmd(cmds, shell, &in_fd, &last_pid))
			return (1);
		cmds = cmds->next;
	}
	while (wait(&status) > 0)
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	return (0);
}
