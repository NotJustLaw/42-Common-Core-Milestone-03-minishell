/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:24:20 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/02/03 17:18:26 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_base(long nbr, int lbase, char *base)
{
	int	count;
	int	rmd;

	count = 0;
	if (nbr < 0)
	{
		count += write(1, "-", 1);
		nbr = -nbr;
	}
	if (nbr >= lbase)
		count += ft_putnbr_base(nbr / lbase, lbase, base);
	rmd = (nbr % lbase);
	write(1, &base[rmd], 1);
	count++;
	return (count);
}
