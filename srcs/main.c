/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/07/15 17:06:49 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

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
			exit(0);
		}
		if (*input)
			add_history(input);
		free_commands(prog_data()->commands);
		prog_data()->commands = NULL;
		parser(input);
		free(input);
	}
	return(0);
}
