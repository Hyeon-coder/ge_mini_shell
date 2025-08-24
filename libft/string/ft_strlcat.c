/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:01:31 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:36:03 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Concatenates two strings with size limit. (크기 제한을 두고 문자열 이어 붙이기)
*/
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	remain_len;
	size_t	i;

	i = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	remain_len = size - dst_len;
	if (!size)
		return (src_len);
	if (dst_len >= size)
		return (src_len + size);
	while (i < (remain_len - 1) && src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
