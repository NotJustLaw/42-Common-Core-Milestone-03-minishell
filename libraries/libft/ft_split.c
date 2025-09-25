/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notjustlaw <notjustlaw@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:16:25 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/09/25 15:08:20 by notjustlaw       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void *ft_free(char **str)
{
    size_t i = 0;
    while (str[i])
        free(str[i++]);
    free(str);
    return (NULL);
}

static int	fill_words(char **newstr, const char *phrase, char c)
{
	size_t i;
	size_t count;

	count = 0;
	while (*phrase)
	{
		while (*phrase && *phrase == c)
			phrase++;
		if (*phrase)
		{
			i = 0;
			while (phrase[i] && phrase[i] != c)
				i++;
			newstr[count] = ft_substr(phrase, 0, i);
			if (!newstr[count])
			{
				return (0);
			}
			count++;
			phrase += i;
		}
	}
	return (1);
}

int	count_words(const char *phrase, char c, char **newstr)
{
	size_t	i;
	size_t	count;

	count = 0;
	while (*phrase)
	{
		while (*phrase && *phrase == c)
			phrase++;
		if (*phrase)
		{
			i = 0;
			while (phrase[i] && phrase[i] != c)
				i++;
			if (i && newstr)
			{
				newstr[count] = ft_substr(phrase, 0, i);
				if (!newstr[count])
					return (ft_free(newstr), 0);
			}
			count++;
			phrase += i;
		}
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char **newstr;
	size_t words;

	if (!s)
		return (NULL);
	words = count_words(s, c, NULL);
	newstr = ft_calloc(words + 1, sizeof(char *));
	if (!newstr)
		return (NULL);

	if (!fill_words(newstr, s, c))
	{
		return (ft_free(newstr));
	}
	return (newstr);
}
