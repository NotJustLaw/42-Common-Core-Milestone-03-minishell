/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:08:45 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/21 17:43:15 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s);

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*frase;

	i = 0;
	if (!s1)
		return (NULL);
	frase = (char *)malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (!frase)
		return (NULL);
	while (i < ft_strlen(s1))
	{
		frase[i] = s1[i];
		i++;
	}
	frase[i] = '\0';
	return (frase);
}
