/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:38:27 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/07/16 17:25:10 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*final;
	const char	*inicio;

	if (dest == src || n == 0)
		return (dest);
	final = (char *)dest;
	inicio = (const char *)src;
	if (n == 0)
		return (dest);
	i = -1;
	while (++i < n)
		final[i] = inicio[i];
	return (final);
}
