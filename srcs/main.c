/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/27 15:09:36 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*prog_data(void)
{
	static t_shell	prog_data;

	return (&prog_data);
}

static void	cleanup_and_exit(t_shell *shell)
{
	free_shell_data(shell);
}

int main(int ac, char **av, char **envp)
{
	char	*input;
	t_shell *shell;

	(void)ac;
	(void)av;
	shell = prog_data();
	shell->envp = copy_envp(envp);
	shell->exit_status = 0;
	shell->is_running = 1;
	shell->commands = NULL;

	while (shell->is_running)
	{
		signals_interactive();
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break;
		}

		if (*input)
		{
			add_history(input);
			parser(input);
			if (shell->commands)
			{
				collect_all_heredocs();
				if (!prog_data()->heredoc_interrupted)
				{
					signals_execution();
					execute_all(shell->commands, shell);
				}
			}
		}
		free(input);
		if (shell->commands)
		{
			free_commands(shell->commands);
			shell->commands = NULL;
		}
	}
	cleanup_and_exit(shell);
	return (shell->exit_status);
}