/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:08:47 by skuhlcke          #+#    #+#             */
/*   Updated: 2025/09/29 21:56:55 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_single_command(t_command *cmds, t_shell *shell)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	ign;

	if (cmds->redirection_failed)
		return (prog_data()->exit_status = 1, 1);
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	if (builtin_chkr(cmds->args) && cmds->input_fd <= 0 && cmds->output_fd <= 0)
	{
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		return (execute_builtin(cmds->args, shell));
	}
	if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0')
	{
		sigaction(SIGINT, &old_int, NULL);
		return (sigaction(SIGQUIT, &old_quit, NULL), shell->exit_status = 0, 0);
	}
	single_command_fork_wait(cmds, shell);
	sigaction(SIGINT, &old_int, NULL);
	return (sigaction(SIGQUIT, &old_quit, NULL), 0);
}

int	execute_pipeline(t_command *cmds, t_shell *shell)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	ign;
	t_command			*it;

	it = cmds;
	while (it)
	{
		if (it->redirection_failed)
			return (prog_data()->exit_status = 1, 1);
		it = it->next;
	}
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	pipeline_fork_and_manage(cmds, shell);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	return (0);
}
