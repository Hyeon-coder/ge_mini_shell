/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:45:16 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:25:49 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Compares two memory areas. (메모리 영역을 비교)
*/
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*tmp1;
	unsigned char	*tmp2;
	int				i;

	tmp1 = (unsigned char *)s1;
	tmp2 = (unsigned char *)s2;
	i = 0;
	while (n > 0)
	{
		if (tmp1[i] != tmp2[i])
			return (tmp1[i] - tmp2[i]);
		n--;
		i++;
	}
	return (0);
}
