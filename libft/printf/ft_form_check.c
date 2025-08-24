/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_form_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <ljh3900@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:37:49 by JuHyeon           #+#    #+#             */
/*   Updated: 2024/11/23 16:31:20 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	parse_char(const char *format, va_list args)
{
	if (*format == 'c')
		return (ft_putchar_rt(va_arg(args, int)));
	if (*format == '%')
		return (ft_putchar_rt('%'));
	return (0);
}

static int	parse_string(const char *format, va_list args)
{
	char	*str;
	void	*ptr;

	if (*format == 's')
	{
		str = va_arg(args, char *);
		return (ft_putstr_rt(str));
	}
	if (*format == 'p')
	{
		ptr = va_arg(args, void *);
		return (ft_putpointer(ptr));
	}
	return (0);
}

static int	parse_number(const char *format, va_list args)
{
	char	*str;
	int		len;

	if (*format == 'd' || *format == 'i')
		str = ft_itoa(va_arg(args, int));
	else if (*format == 'u')
		str = ft_itoa_base(va_arg(args, unsigned int), "0123456789");
	else if (*format == 'x')
		str = ft_itoa_base(va_arg(args, unsigned int), "0123456789abcdef");
	else if (*format == 'X')
		str = ft_itoa_base(va_arg(args, unsigned int), "0123456789ABCDEF");
	else
		return (0);
	len = ft_putstr_rt(str);
	free(str);
	return (len);
}

int	ft_form_check(const char *format, va_list args)
{
	int	len;

	len = parse_char(format, args);
	if (len == 0)
		len = parse_string(format, args);
	if (len == 0)
		len = parse_number(format, args);
	return (len);
}
