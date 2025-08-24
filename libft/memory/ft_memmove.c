/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:01:42 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:26:10 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Moves memory safely. (메모리를 안전하게 이동)
*/
void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char	*dst1;
	unsigned char	*src1;

	dst1 = (unsigned char *)dst;
	src1 = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	if (dst1 < src1)
		ft_memcpy(dst, src, n);
	else
	{
		while (n > 0)
		{
			n--;
			dst1[n] = src1[n];
		}
	}
	return (dst);
}
