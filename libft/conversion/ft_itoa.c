/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 19:16:13 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:49:35 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Counts the number of digits in an integer. (정수의 자리수를 계산)
*/
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

/*
	Converts an integer to a string. (정수를 문자열로 변환)
*/
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
