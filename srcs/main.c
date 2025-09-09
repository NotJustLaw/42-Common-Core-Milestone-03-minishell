/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/09 17:38:38 by marvin           ###   ########.fr       */
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

int main(int ac, char **av)
{
	char	*input = NULL;

	(void)ac;
	(void)av;
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
		//we need to check if its a built in, if its not then we call out pid_t pid = fork()
		//if pid == 0  execve and reset_signals and exit 1 
		//else waitpid(pid, &status, 0);
		//and so if (WIFEXISTED(status))-set_exit_status(WEXITSTATUS(status))
		//else if (WIFSGINALED(status))-set_exit_status(128 + WTERMSIG(status))
		free(input);
	}
	return(0);
}
