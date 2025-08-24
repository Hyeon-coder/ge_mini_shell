/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 00:31:35 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:27:30 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Prints a string to a file descriptor. (문자열을 파일 디스크립터에 출력)
*/
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = -1;
	while (s[++i])
		write(fd, &s[i], 1);
}
