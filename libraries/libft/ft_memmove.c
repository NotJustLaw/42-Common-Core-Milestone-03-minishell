/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:13:21 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 10:54:44 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*t_dst;
	char	*t_src;
	size_t	i;

	t_dst = (char *)dest;
	t_src = (char *)src;
	i = 0;
	if (dest == src || n == 0)
		return (dest);
	if (t_dst >= t_src)
	{
		while (n-- > 0)
			t_dst[n] = t_src[n];
	}
	else
	{
		while (i < n)
		{
			t_dst[i] = t_src[i];
			i++;
		}
	}
	return (dest);
}
