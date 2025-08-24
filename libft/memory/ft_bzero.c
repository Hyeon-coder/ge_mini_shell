/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:50:50 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:25:59 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Sets memory to zero. (메모리를 0으로 초기화)
*/
void	ft_bzero(void *s, size_t n)
{
	unsigned char	*tmp;

	tmp = (unsigned char *)s;
	while (n > 0)
	{
		*(tmp++) = '\0';
		n--;
	}
}
