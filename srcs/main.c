/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justlaw <justlaw@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/07/02 13:39:18 by justlaw          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

// t_shell	*prog_data(void)
// {
// 	static t_shell	prog_data;

// 	return (&prog_data);
// }

static volatile int keepRunning = 1;

void	int_handler(int dummy)
{
	(void)dummy;
	printf("minishell > ");
	keepRunning = 0;
}

int main(int ac, char **av)
{
	char	*input = NULL;
	t_shell	*shell;
	
	(void)ac;
	(void)av;
	shell = prog_data();
	signal(SIGINT, int_handler);
	while (keepRunning)
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
		execute_all(prog_data()->commands, shell);
		free(input);
	}
	// -la";
	// char *str = ft_strdup("echo<<p hello \"| wc\" | ls -la");
	//char *str2 = "echo2hello2\"| wc\"2|2ls2-la";
	// echo<<
	

	return(0);
}

t_shell	*prog_data(void)
{
	static t_shell	prog_data;

	return (&prog_data);
}
