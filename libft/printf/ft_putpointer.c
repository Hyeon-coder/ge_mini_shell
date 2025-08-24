/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <ljh3900@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:07:28 by JuHyeon           #+#    #+#             */
/*   Updated: 2024/11/23 16:40:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putpointer(void *ptr)
{
	int		len;
	char	*str;

	if (!ptr)
	{
		ft_putstr_rt("(nil)");
		return (ft_strlen("(nil)"));
	}
	ft_putstr_rt("0x");
	str = ft_itoa_base((unsigned long)ptr, "0123456789abcdef");
	if (!str)
		return (0);
	len = ft_putstr_rt(str);
	free(str);
	return (len + 2);
}
