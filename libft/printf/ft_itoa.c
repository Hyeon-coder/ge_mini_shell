/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <ljh3900@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 19:16:13 by JuHyeon           #+#    #+#             */
/*   Updated: 2024/11/15 20:32:27 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "libft.h"
#include "ft_printf.h"

int	cnt_num(int n)
{
	int	cnt;

	if (n <= 0)
		cnt = 1;
	else
		cnt = 0;
	while (n)
	{
		n /= 10;
		cnt++;
	}
	return (cnt);
}

char	*ft_itoa(int n)
{
	char	*rst;
	int		len;
	long	num;

	num = n;
	len = cnt_num(num);
	rst = (char *)malloc((len + 1) * sizeof(char));
	if (!rst)
		return (0);
	rst[len] = '\0';
	if (num < 0)
	{
		rst[0] = '-';
		num = -num;
	}
	else if (num == 0)
		rst[0] = '0';
	while (num > 0)
	{
		rst[--len] = (num % 10) + '0';
		num /= 10;
	}
	return (rst);
}
