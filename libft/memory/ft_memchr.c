/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:03:53 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:52:18 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Searches for a character in a memory block. (메모리 블록에서 특정 문자를 검색)
*/
void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*tmp;
	unsigned char		target;

	tmp = (const unsigned char *)s;
	target = (unsigned char)c;
	while (n > 0)
	{
		if (*tmp == target)
			return ((char *)tmp);
		tmp++;
		n--;
	}
	return (0);
}
