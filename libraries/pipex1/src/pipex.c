/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:55:15 by justlaw           #+#    #+#             */
/*   Updated: 2025/06/04 18:33:38 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char *av[], char **envp)
{
	if (ac < 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (1);
	}
	if (ft_strncmp(av[1], "here_doc", ft_strlen("here_doc") + 1) == 0)
	{
		if (ac < 6)
		{
			ft_putstr_fd("Usage: ./pipex here_doc LIMITER cmd1 \
				[cmd2 ... cmdN] outfile\n", 2);
			return (1);
		}
		handle_here_doc_mode(ac, av, envp);
	}
	else
	{
		if (ac == 5)
			handle_two_command_mode(ac, av, envp);
		else
			handle_multi_pipe_mode(ac, av, envp);
	}
	return (0);
}

void	handle_two_command_mode(int ac, char *av[], char **envp)
{
	t_fd_list	fds;

	(void)ac;
	fds.fd_in = fd_in_handler(av);
	if (fds.fd_in < 0)
	{
		perror("open infile error");
		exit(1);
	}
	fds.fd_out = fd_out_handler(av);
	if (fds.fd_out < 0)
	{
		perror("open outfile error");
		close(fds.fd_in);
		exit(1);
	}
	if (pipe_generator(fds.pipefd))
		exit(1);
	fds.envp = envp;
	forking_process(&fds, av, envp);
	close(fds.fd_in);
	close(fds.fd_out);
}

void	forking_process(t_fd_list *fds, char *av[], char **envp)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == 0)
		child1(fds, av, envp);
	pid2 = fork();
	if (pid2 == 0)
		child2(fds, av, envp);
	close(fds->pipefd[0]);
	close(fds->pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
