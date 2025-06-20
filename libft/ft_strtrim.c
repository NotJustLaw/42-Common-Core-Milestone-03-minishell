/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:17:23 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/12 18:31:43 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_set(const char *frase, char c)
{
	int	i;

	i = 0;
	while (frase[i])
	{
		if (frase[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int		i;
	int		j;
	int		k;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	k = 0;
	i = 0;
	j = ft_strlen(s1) - 1;
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	while (check_set (set, s1[i]))
		i++;
	while (j >= i && check_set (set, s1[j]))
		j--;
	trimmed = malloc(j - i + 2);
	if (!trimmed)
		return (NULL);
	while (i <= j)
	{
		trimmed[k++] = s1[i++];
	}
	trimmed[k] = '\0';
	return (trimmed);
}
