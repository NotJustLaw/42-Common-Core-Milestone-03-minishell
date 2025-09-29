/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:32:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/29 19:18:52 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*prog_data(void)
{
	static t_shell	prog_data;

	return (&prog_data);
}

static void	cleanup_and_exit(t_shell *shell)
{
	free_shell_data(shell);
}

int main(int ac, char **av, char **envp)
{
	char	*input;
	t_shell *shell;

	(void)ac;
	(void)av;
	shell = prog_data();
	shell->envp = copy_envp(envp);
	shell->exit_status = 0;
	shell->is_running = 1;
	shell->commands = NULL;

	while (shell->is_running)
	{
		signals_interactive();
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break;
		}

		if (*input)
		{
			add_history(input);
			parser(input);
			if (shell->commands)
			{
				collect_all_heredocs();
				if (!prog_data()->heredoc_interrupted)
				{
					signals_execution();
					execute_all(shell->commands, shell);
				}
			}
		}
		free(input);
		if (shell->commands)
		{
			free_commands(shell->commands);
			shell->commands = NULL;
		}
	}
	cleanup_and_exit(shell);
	return (shell->exit_status);
}


// minishell > << EOF cat | << bye cat | << lol cat
// > $HOME
// > EOF
// > EOF
// > EOF
// > lol
// > lol
// > lol
// > bye
// > bye
// > EOF
// > lol
// ==63485== Memcheck, a memory error detector
// ==63485== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
// ==63485== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
// ==63485== Command: /usr/bin/cat
// ==63485== 
// ==63486== Memcheck, a memory error detector
// ==63486== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
// ==63486== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
// ==63486== Command: /usr/bin/cat
// ==63486== 
// ==63487== Memcheck, a memory error detector
// ==63487== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
// ==63487== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
// ==63487== Command: /usr/bin/cat
// ==63487== 
// ==63485== 
// ==63485== Process terminating with default action of signal 13 (SIGPIPE)
// ==63485==    at 0x49858F7: write (write.c:26)
// ==63485==    by 0x10DAE7: ??? (in /usr/bin/cat)
// ==63485==    by 0x10B4C3: ??? (in /usr/bin/cat)
// ==63485==    by 0x489AD8F: (below main) (libc_start_call_main.h:58)
// ==63485== 
// ==63485== FILE DESCRIPTORS: 5 open (3 std) at exit.
// ==63485== Open file descriptor 5:
// ==63485==    <inherited from parent>
// ==63485== 
// ==63485== Open file descriptor 4:
// ==63485==    <inherited from parent>
// ==63485== 
// ==63485== 
// ==63485== HEAP SUMMARY:
// ==63485==     in use at exit: 139,227 bytes in 30 blocks
// ==63485==   total heap usage: 31 allocs, 1 frees, 139,232 bytes allocated
// ==63485== 
// ==63485== LEAK SUMMARY:
// ==63485==    definitely lost: 0 bytes in 0 blocks
// ==63485==    indirectly lost: 0 bytes in 0 blocks
// ==63485==      possibly lost: 0 bytes in 0 blocks
// ==63485==    still reachable: 139,227 bytes in 30 blocks
// ==63485==         suppressed: 0 bytes in 0 blocks
// ==63485== Reachable blocks (those to which a pointer was found) are not shown.
// ==63485== To see them, rerun with: --leak-check=full --show-leak-kinds=all
// ==63485== 
// ==63485== For lists of detected and suppressed errors, rerun with: -s
// ==63485== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
// ==63486== 
// ==63486== Process terminating with default action of signal 13 (SIGPIPE)
// ==63486==    at 0x49858F7: write (write.c:26)
// ==63486==    by 0x10DAE7: ??? (in /usr/bin/cat)
// ==63486==    by 0x10B4C3: ??? (in /usr/bin/cat)
// ==63486==    by 0x489AD8F: (below main) (libc_start_call_main.h:58)
// ==63486== 
// ==63486== FILE DESCRIPTORS: 5 open (3 std) at exit.
// ==63486== Open file descriptor 5:
// ==63486==    <inherited from parent>
// ==63486== 
// ==63486== Open file descriptor 3:
// ==63486==    <inherited from parent>
// ==63486== 
// ==63486== 
// ==63486== HEAP SUMMARY:
// ==63486==     in use at exit: 139,227 bytes in 30 blocks
// ==63486==   total heap usage: 31 allocs, 1 frees, 139,232 bytes allocated
// ==63486== 
// ==63486== LEAK SUMMARY:
// ==63486==    definitely lost: 0 bytes in 0 blocks
// ==63486==    indirectly lost: 0 bytes in 0 blocks
// ==63486==      possibly lost: 0 bytes in 0 blocks
// ==63486==    still reachable: 139,227 bytes in 30 blocks
// ==63486==         suppressed: 0 bytes in 0 blocks
// ==63486== Reachable blocks (those to which a pointer was found) are not shown.
// ==63486== To see them, rerun with: --leak-check=full --show-leak-kinds=all
// ==63486== 
// ==63486== For lists of detected and suppressed errors, rerun with: -s
// ==63486== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
// bye
// EOF
// ==63487== 
// ==63487== FILE DESCRIPTORS: 2 open (0 std) at exit.
// ==63487== Open file descriptor 4:
// ==63487==    <inherited from parent>
// ==63487== 
// ==63487== Open file descriptor 3:
// ==63487==    <inherited from parent>
// ==63487== 
// ==63487== 
// ==63487== HEAP SUMMARY:
// ==63487==     in use at exit: 0 bytes in 0 blocks
// ==63487==   total heap usage: 31 allocs, 31 frees, 139,232 bytes allocated
// ==63487== 
// ==63487== All heap blocks were freed -- no leaks are possible
// ==63487== 
// ==63487== For lists of detected and suppressed errors, rerun with: -s
// ==63487== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
// minishell > 
