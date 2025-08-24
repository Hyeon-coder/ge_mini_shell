/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <ljh3900@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:17:12 by JuHyeon           #+#    #+#             */
/*   Updated: 2024/11/23 16:30:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>

int		ft_putchar_rt(char c);
int		ft_putstr_rt(char *str);
int		ft_form_check(const char *format, va_list args);
int		ft_printf(const char *format, ...);
int		ft_putpointer(void *ptr);
size_t	ft_strlen(const char *s);
char	*ft_itoa(int n);
char	*ft_itoa_base(unsigned long num, char *base);
char	*ft_strchr(const char *str, int c);

#endif
