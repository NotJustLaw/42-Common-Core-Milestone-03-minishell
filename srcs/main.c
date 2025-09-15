/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/16 01:06:16 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*prog_data(void)
{
	static t_shell	prog_data;

	return (&prog_data);
}

int main(int ac, char **av, char **envp)
{
	char	*input = NULL;

	(void)ac;
	(void)av;
	prog_data()->envp = copy_envp(envp);
	prog_data()->exit_status = 0;
	prog_data()->is_running = 1;
	full_sighandler();
	while (1)
	{
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (*input)
			add_history(input);
		free_commands(prog_data()->commands);
		prog_data()->commands = NULL;
		parser(input);
		collect_all_heredocs();
		execute_all(prog_data()->commands, prog_data());
		free(input);
	}
	return(0);
}
