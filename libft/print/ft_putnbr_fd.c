/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 00:57:43 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:27:41 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Prints a number to a file descriptor. (숫자를 파일 디스크립터에 출력)
*/
void	ft_putnbr_fd(int n, int fd)
{
	char	temp;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
	}
	while (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		n %= 10;
	}
	if (n < 10)
	{
		temp = n + '0';
		write(fd, &temp, 1);
	}
}
