/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/25 19:16:33 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*prog_data(void)
{
	static t_shell	prog_data;

	return (&prog_data);
}

// RENAMED: This is the single source of truth for cleanup.
static void	cleanup_and_exit(t_shell *shell)
{
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->envp)
		free_double_ptr(shell->envp);
	rl_clear_history();
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
		if (!input) // Ctrl+D exits the loop
		{
			printf("exit\n");
			break;
		}

		if (*input)
		{
			add_history(input);
			parser(input); // Allocates shell->commands
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

		// At the END of the loop, free the commands from the CURRENT loop.
		if (shell->commands)
		{
			free_commands(shell->commands);
			shell->commands = NULL;
		}
	}

	// All cleanup is now handled by this single function call before exiting.
	cleanup_and_exit(shell);
	return (shell->exit_status);
}