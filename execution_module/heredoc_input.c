/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:15:00 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/29 22:12:38 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static int	write_heredoc_line(char *line, const char *limiter,
		size_t len, t_command *cmd, int fd)
{
	char	*orig;

	if ((ft_strncmp(line, limiter, len) == 0) && line[len] == '\0')
		return (free(line), 1);
	if (cmd && cmd->heredoc_expand)
	{
		orig = line;
		line = expand_argument_heredoc(orig);
		free(orig);
	}
	if (line)
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (0);
}
