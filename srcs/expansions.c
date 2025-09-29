/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:50:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/29 20:56:34 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	handle_unquoted(char ***new_args, int *size, char *expanded)
{
	char	**split;

	split = ft_split(expanded, ' ');
	free(expanded);
	if (!split)
		return (false);
	if (!add_split_words(new_args, size, split))
		return (false);
	return (true);
}

static bool	process_one_arg(t_command *cmd,
			char ***new_args, int *size, int i)
{
	char	*expanded;

	expanded = expand_argument(cmd->args[i], 0, false, false);
	if (!expanded)
		return (false);
	if (is_unquoted_var(cmd->args[i]))
		return (handle_unquoted(new_args, size, expanded));
	if (!add_one_word(new_args, size, expanded))
	{
		free(expanded);
		return (false);
	}
	return (true);
}

static bool	expand_args(t_command *cmd)
{
	char	**new_args;
	int		new_size;
	int		i;

	new_args = NULL;
	new_size = 0;
	i = -1;
	while (cmd->args[++i])
		if (!process_one_arg(cmd, &new_args, &new_size, i))
			return (free_double_ptr(new_args), false);
	free_double_ptr(cmd->args);
	cmd->args = ft_realloc(new_args, new_size * sizeof(char *),
			(new_size + 1) * sizeof(char *));
	if (!cmd->args)
		return (free_double_ptr(new_args), false);
	cmd->args[new_size] = NULL;
	return (true);
}

static void	expand_files(t_command *cmd)
{
	char	*tmp;

	if (cmd->infile)
	{
		tmp = expand_argument(cmd->infile, 0, false, false);
		if (tmp)
		{
			free(cmd->infile);
			cmd->infile = tmp;
		}
	}
	if (cmd->outfile)
	{
		tmp = expand_argument(cmd->outfile, 0, false, false);
		if (tmp)
		{
			free(cmd->outfile);
			cmd->outfile = tmp;
		}
	}
}

void	expansion_trade(void)
{
	t_command	*cmd;

	cmd = prog_data()->commands;
	while (cmd)
	{
		if (cmd->args && !expand_args(cmd))
			return ;
		expand_files(cmd);
		cmd = cmd->next;
	}
}
