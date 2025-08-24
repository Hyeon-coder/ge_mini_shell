/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:26:43 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:52:04 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Copies memory from one location to another. (메모리 블록을 다른 메모리 블록으로 복사)
*/
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*dst1;
	unsigned const char	*src1;

	i = 0;
	dst1 = dst;
	src1 = src;
	while (i < n)
	{
		dst1[i] = src1[i];
		i++;
	}
	return (dst);
}
