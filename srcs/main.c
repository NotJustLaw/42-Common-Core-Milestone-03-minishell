/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/24 19:10:47 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_exit_status = 0;

int	get_exit_status(void)
{
	return (g_exit_status);
}

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
			free_commands(prog_data()->commands);
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
