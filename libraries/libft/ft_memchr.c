/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:42:10 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 17:49:18 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*frase;
	unsigned char	*temp;

	i = 0;
	frase = (unsigned char *)s;
	if (!s)
	{
		temp = (unsigned char *)s;
		*temp = 0;
	}
	if (!frase)
		return (NULL);
	while (i < n)
	{
		if (frase[i] == (unsigned char)c)
			return ((void *)&frase[i]);
		i++;
	}
	return (NULL);
}
