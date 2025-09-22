/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:31:48 by justlaw           #+#    #+#             */
/*   Updated: 2025/06/04 14:56:53 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	fd_in_handler(char *av[])
{
	return (open(av[1], O_RDONLY));
}

int	fd_out_handler(char *av[])
{
	return (open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644));
}

int	pipe_generator(int pipefd[2])
{
	if (pipe(pipefd) == -1)
		return (perror("pipe error"), 1);
	return (0);
}
