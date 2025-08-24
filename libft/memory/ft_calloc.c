/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:38:51 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:52:23 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Allocates memory and initializes it to zero. (메모리를 할당하고 0으로 초기화)
*/
void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count && size > (size_t)-1 / count)
		return (NULL);
	ptr = (void *)malloc(count * size);
	if (ptr == 0)
		return (ptr);
	ft_bzero(ptr, size * count);
	return (ptr);
}
