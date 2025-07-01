/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:40:15 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 17:52:46 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	destl;
	size_t	sorcl;
	size_t	i;

	i = 0;
	destl = ft_strlen(dst);
	sorcl = ft_strlen(src);
	if (size <= destl)
		return (size + sorcl);
	while (destl + i + 1 < size && src[i] != '\0')
	{
		dst[destl + i] = src[i];
		i++;
	}
	dst[destl + i] = '\0';
	return (destl + sorcl);
}
