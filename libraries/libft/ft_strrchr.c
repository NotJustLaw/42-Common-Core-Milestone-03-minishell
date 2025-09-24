/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:59:23 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/18 15:34:06 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*frase;

	frase = (char *)s;
	i = 0;
	while (s[i])
		i++;
	while (i > 0)
	{
		if (s[i] == (unsigned char)c)
			return (&frase[i]);
		i--;
	}
	if (s[i] == (unsigned char)c)
		return (&frase[i]);
	return (0);
}
