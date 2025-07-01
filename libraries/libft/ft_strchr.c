/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:59:23 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 15:33:25 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*dest;

	i = 0;
	dest = (char *)s;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			return (&dest[i]);
		i++;
	}
	if (s[i] == (unsigned char)c)
		return (&dest[i]);
	return (NULL);
}
