/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/06/28 13:14:09 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

// t_shell	*prog_data(void)
// {
// 	static t_shell	prog_data;

// 	return (&prog_data);
// }

static volatile int keepRunning = 1;

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
