/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/29 18:57:06 by henrique-re      ###   ########.fr       */
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

static void	execute_if_ready(t_shell *shell)
{
	if (!shell->commands)
		return ;
	collect_all_heredocs();
	if (!prog_data()->heredoc_interrupted)
	{
		signals_execution();
		execute_all(shell->commands, shell);
	}
	free_commands(shell->commands);
	shell->commands = NULL;
}

static void	read_and_parse(t_shell *shell)
{
	char	*input;

	signals_interactive();
	input = readline("minishell > ");
	if (!input)
	{
		printf("exit\n");
		shell->is_running = 0;
		return ;
	}
	if (*input)
	{
		add_history(input);
		parser(input);
		execute_if_ready(shell);
	}
	free(input);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = prog_data();
	shell->envp = copy_envp(envp);
	shell->exit_status = 0;
	shell->is_running = 1;
	shell->commands = NULL;
	while (shell->is_running)
		read_and_parse(shell);
	cleanup_and_exit(shell);
	return (shell->exit_status);
}
