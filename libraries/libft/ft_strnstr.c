/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:44:03 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 13:32:34 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	l;

	i = 0;
	l = 0;
	if (little[l] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		l = 0;
		while (big[i + l] == little[l] && i + l < len)
		{
			l++;
			if (!little[l])
				return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}
