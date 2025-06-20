/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:33:14 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 17:49:30 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*frase1;
	unsigned char	*frase2;

	frase1 = (unsigned char *)s1;
	frase2 = (unsigned char *)s2;
	i = 0;
	if (!frase1 || !frase2)
		frase1[i] = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && frase1[i] == frase2[i])
		i++;
	return ((unsigned char)frase1[i] - (unsigned char)frase2[i]);
}
