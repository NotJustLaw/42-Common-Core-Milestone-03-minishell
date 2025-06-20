/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:01:50 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/02/06 15:59:02 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;

	if (!str)
		return (-1);
	va_start(args, str);
	count = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			count += ft_checkpercentage(str, args);
		}
		else
		{
			write(1, &*str, 1);
			count++;
		}
		str++;
	}
	va_end(args);
	return (count);
}

int	ft_checkpercentage(const char *str, va_list	args)
{
	int	count;

	if (*str == 'c')
		count = ft_putchar(va_arg(args, int));
	if (*str == 's')
		count = ft_putstr(va_arg(args, char *));
	if (*str == 'p')
		count = ft_prtptr(va_arg(args, unsigned long long), LHEX, 0);
	if (*str == 'd' || *str == 'i')
		count = ft_putnbr_base(va_arg(args, int), 10, "0123456789");
	if (*str == 'u')
		count = ft_putnbr_base(va_arg(args, unsigned int), 10, "0123456789");
	if (*str == 'x')
		count = ft_putnbr_base(va_arg(args, unsigned int), 16, LHEX);
	if (*str == 'X')
		count = ft_putnbr_base(va_arg(args, unsigned int), 16, UHEX);
	if (*str == '%')
		count = ft_putchar('%');
	return (count);
}

/* int main() {
    // Test case inputs
    int num = 12345;
    unsigned int u_num = 123456789;
    unsigned long ptr = (unsigned long)&num;
    char c = 'A';
    char *str = "Hello, world!";
    char *null_str = NULL;
    int large_num = 2147483647;
    int small_num = -2147483648;
    unsigned int u_large_num = 4294967295;
    unsigned int negative_u_num = -12345; 

    // Test various format specifiers
	ft_printf("%d\n", ft_printf(NULL));
	printf("%d\n", printf(NULL));
    ft_printf("Character: %c\n", c);  
    ft_printf("String: %s\n", str); 
    ft_printf("String (NULL): %s\n", null_str); 
    ft_printf("Pointer: %p\n", ptr);  
    ft_printf("Pointer (NULL): %p\n", NULL);  
    ft_printf("Signed integer: %d\n", num);  
    ft_printf("Signed integer (large): %d\n", large_num);
    ft_printf("Signed integer (small): %d\n", small_num);
    ft_printf("Unsigned integer: %u\n", u_num);  
    ft_printf("Unsigned integer (large): %u\n", u_large_num);
    ft_printf("Unsigned integer (negative cast): %u\n", negative_u_num);
    ft_printf("Hexadecimal (lowercase): %x\n", u_num);  
    ft_printf("Hexadecimal (lowercase, large): %x\n", u_large_num);
    ft_printf("Hexadecimal (uppercase): %X\n", u_num);  
    ft_printf("Hexadecimal (uppercase, large): %X\n", u_large_num);
    ft_printf("Hexadecimal (negative): %x\n", negative_u_num);
    ft_printf("Percentage sign: %%\n");

    ft_printf("Invalid Specifier: %z\n");
	
    char *empty_str = "";
    ft_printf("Empty String: %s\n", empty_str);

    return 0;
} */