/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:42:23 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 17:49:46 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*frase;

	i = (size_t)start;
	j = 0;
	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (ft_strlen(s) - start < len)
		len = ft_strlen(s) - start;
	frase = malloc(len + 1);
	if (!frase)
		return (NULL);
	while (i < len + (size_t)start)
	{
		frase[j] = s[i];
		j++;
		i++;
	}
	frase[j] = '\0';
	return (frase);
}
