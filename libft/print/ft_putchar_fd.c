/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 21:59:12 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:28:09 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Prints a character to a file descriptor. (문자를 파일 디스크립터에 출력)
*/
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
