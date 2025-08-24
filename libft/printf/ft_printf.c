/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <ljh3900@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:20:32 by JuHyeon           #+#    #+#             */
/*   Updated: 2024/11/19 19:31:19 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		printed;

	printed = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (ft_strchr("cspdiuxX%", *format))
				printed += ft_form_check(format, args);
		}
		else
			printed += ft_putchar_rt(*format);
		format++;
	}
	va_end(args);
	return (printed);
}
/*
int	main(void)
{
	char	*ptr = "Hello, World";
	int		i = -42;
	int		a = 999;
	printf("%c\n", 'c');
	ft_printf("ft : %c\n\n", 'c');
	printf("%s\n", ptr);
	ft_printf("ft : %s\n\n", ptr);
	printf("%p\n", ptr);
	ft_printf("ft : %p\n\n", ptr);
	printf("%d\n", i);
	ft_printf("ft : %d\n\n", i);
	printf("%i\n", i);
	ft_printf("ft : %i\n\n", i);
	printf("%u\n", i);
	ft_printf("ft : %u\n\n", i);
	printf("%x\n", a);
	ft_printf("ft : %x\n\n", a);
	printf("%X\n", a);
	ft_printf("ft : %X\n\n", a);
	printf("42%%\n");
	ft_printf("ft : 42%%\n\n");
}
*/
