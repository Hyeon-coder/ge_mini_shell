/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:34:18 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/24 18:17:29 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_cntlen(unsigned long tmp, int base_len)
{
	int	len;

	if (tmp == 0)
		len = 1;
	else
		len = 0;
	while (tmp)
	{
		tmp /= base_len;
		len++;
	}
	return (len);
}

char	*ft_itoa_base(unsigned long num, char *base)
{
	char			*str;
	unsigned long	tmp;
	int				len;
	int				base_len;

	base_len = ft_strlen(base);
	tmp = num;
	len = ft_cntlen(tmp, base_len);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = base[num % base_len];
		num /= base_len;
	}
	return (str);
}
