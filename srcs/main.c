/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/25 13:28:51 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*prog_data(void)
{
	static t_shell	prog_data;

	return (&prog_data);
}

static void	final_cleanup(t_shell *shell)
{
	if (shell->envp)
		free_double_ptr(shell->envp);
	if (shell->commands)
		free_commands(shell->commands);
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

	full_sighandler();
	while (shell->is_running)
	{
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break; // Break the loop, don't exit()
		}
		if (*input)
		{
			add_history(input);
			parser(input);
			if (shell->commands)
			{
				collect_all_heredocs();
				execute_all(shell->commands, shell);
			}
		}
		free(input);
		if (shell->commands)
		{
			free_commands(shell->commands);
			shell->commands = NULL;
		}
	}
	final_cleanup(shell);
	return (shell->exit_status);
}
