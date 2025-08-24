/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:42:54 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:32:49 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Copies a string with size limit. (크기 제한을 두고 문자열 복사)
*/
size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	srcsize;
	size_t	i;

	srcsize = 0;
	i = 0;
	if (!src || !dest)
		return (0);
	srcsize = ft_strlen(src);
	if (size != 0)
	{
		while (src[i] != '\0' && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (srcsize);
}
