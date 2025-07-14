/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/07/14 17:35:18 by hcarrasq         ###   ########.fr       */
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
	// echo
	// hello
	// \"| wc\"
	// |
	// ls
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
	// -la";
	// char *str = ft_strdup(	);
	//char *str2 = "echo2hello2\"| wc\"2|2ls2-la";
	// echo<<
	

	return(0);
}

