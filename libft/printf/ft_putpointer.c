/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:07:28 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/24 18:17:45 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
