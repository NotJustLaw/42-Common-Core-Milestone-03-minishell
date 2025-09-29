/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exapansion_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:49:04 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/29 20:57:50 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_unquoted_var(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] != '$')
		return (false);
	while (arg[++i])
	{
		if (arg[i] == '\'' || arg[i] == '\"')
			return (false);
	}
	return (true);
}

char	**add_to_array(char **array, int *size, char *str)
{
	char	**new_array;

	new_array = ft_realloc(array, (*size) * sizeof(char *),
			(*size + 1) * sizeof(char *));
	if (!new_array)
	{
		free(str);
		return (NULL);
	}
	new_array[*size] = str;
	(*size)++;
	return (new_array);
}

bool	add_one_word(char ***new_args, int *size, char *word)
{
	*new_args = add_to_array(*new_args, size, word);
	if (!*new_args)
		return (false);
	return (true);
}

bool	add_split_words(char ***new_args, int *size, char **split)
{
	int		j;
	bool	ok;

	j = 0;
	ok = true;
	while (split[j] && ok)
	{
		if (!add_one_word(new_args, size, split[j]))
			ok = false;
		j++;
	}
	while (split[j])
		free(split[j++]);
	free(split);
	return (ok);
}
