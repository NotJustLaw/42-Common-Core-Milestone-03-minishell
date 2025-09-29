/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argument.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henrique-reis <henrique-reis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:51:12 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/29 20:54:40 by henrique-re      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	append_var_value(char **res, const char *arg, int *i)
{
	int		start;
	int		len;
	char	*name;
	char	*val;

	start = *i;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
		(*i)++;
	len = *i - start;
	name = ft_strndup(&arg[start], len);
	if (!name)
		return (false);
	val = get_env_value(prog_data()->envp, name);
	free(name);
	if (val && strncat_realloc(res, val, strlen(val)) == -1)
		return (false);
	return (true);
}

static bool	append_status(char **res, int *i)
{
	char	*status;

	status = ft_itoa(prog_data()->exit_status);
	if (!status)
		return (false);
	if (strncat_realloc(res, status, strlen(status)) == -1)
		return (free(status), false);
	free(status);
	(*i)++;
	return (true);
}

static bool	expand_dollar(char **res, const char *arg, int *i)
{
	(*i)++;
	if (!arg[*i] || ft_strchr(" \t'\"", arg[*i]))
		return (strncat_realloc(res, "$", 1) != -1);
	if (arg[*i] == '?')
		return (append_status(res, i));
	if (ft_isalpha(arg[*i]) || arg[*i] == '_')
		return (append_var_value(res, arg, i));
	return (strncat_realloc(res, "$", 1) != -1);
}

char	*expand_argument(const char *arg, int i, bool in_s, bool in_d)
{
	char	*res;

	res = calloc(1, 1);
	if (!res)
		return (NULL);
	while (arg[i])
	{
		if (arg[i] == '\'' && !in_d)
			in_s = !in_s;
		if (arg[i] == '\'' && !in_d)
			i++;
		else if (arg[i] == '"' && !in_s)
		{
			in_d = !in_d;
			i++;
		}
		else if (arg[i] == '$' && !in_s)
		{
			if (!expand_dollar(&res, arg, &i))
				return (free(res), NULL);
		}
		else if (strncat_realloc(&res, &arg[i++], 1) == -1)
			return (free(res), NULL);
	}
	return (res);
}
