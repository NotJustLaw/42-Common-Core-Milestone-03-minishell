/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:19:55 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/05/07 11:20:40 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *nptr)
{
	int		count;
	int		i;
	long	num;
	long	max;

	max = (long)INT_MAX + 1;
	num = 0;
	i = 0;
	count = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i++] == '-')
			count *= -1;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (num > max)
			break ;
		num = num * 10 + (nptr[i] - 48);
		i++;
	}
	return (num * count);
}
