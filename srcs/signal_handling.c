/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 16:43:10 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/25 18:02:53 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Handler for the main interactive prompt.
static void	sigint_interactive_handler(int sig)
{
	(void)sig;
	prog_data()->exit_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Handler for heredoc. It is simple and uses the ioctl trick.
void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	prog_data()->heredoc_interrupted = 1;
    write(STDOUT_FILENO, "\n", 1);
    close(STDIN_FILENO);
}

void	signals_interactive(void)
{
	struct sigaction sa;
	sa.sa_handler = sigint_interactive_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	signals_execution(void)
{
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}