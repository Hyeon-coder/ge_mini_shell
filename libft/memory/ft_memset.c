/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:01:05 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:51:55 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Fills a block of memory with a specific value. (메모리를 특정 값으로 채움)
*/
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*tmp;

	tmp = (unsigned char *)s;
	while (n > 0)
	{
		*(tmp++) = (unsigned char)c;
		n--;
	}
	return (s);
}
