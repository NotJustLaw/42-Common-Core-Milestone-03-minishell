/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:34:51 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/19 11:32:40 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbrsz(int nbr);

char	*ft_itoa(int n)
{
	size_t		digits;
	char		*result;
	long int	nbr;

	nbr = n;
	digits = ft_nbrsz(n);
	if (nbr == INT_MIN)
		return (ft_strdup("-2147483648"));
	if (n < 0)
		nbr = -nbr;
	result = malloc(sizeof(char) * (digits + 1));
	if (!result)
		return (NULL);
	*(result + digits) = 0;
	while (digits--)
	{
		*(result + digits) = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	if (n < 0)
		*(result + 0) = '-';
	return (result);
}

int	ft_nbrsz(int nbr)
{
	size_t	count;

	count = 1;
	if (nbr < 0)
	{
		nbr = -nbr;
		count++;
	}
	while (nbr >= 10)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}
