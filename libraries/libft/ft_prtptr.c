/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prtptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:56:08 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/02/03 17:18:34 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_prtptr(unsigned long long int n, char *base, int flag)
{
	int	count;
	int	rmd;

	count = 0;
	if (n == 0)
		return (write(1, "(nil)", 5));
	if (flag != 1)
	{
		write (1, "0x", 2);
		count = 2;
		flag = 1;
	}
	if (n >= 16)
		count += ft_prtptr(n / 16, base, 1);
	rmd = (n % 16);
	write(1, &base[rmd], 1);
	count++;
	return (count);
}
