/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:36:44 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:51:19 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Finds the last occurrence of a character in a string.
	(문자열에서 특정 문자의 마지막 위치 찾기)
*/
char	*ft_strrchr(const char *s, int c)
{
	char	*i;

	i = 0;
	while (*s)
	{
		if (*s == c)
			i = (char *)s;
		++s;
	}
	if (i)
		return (i);
	if (!c)
		return ((char *)s);
	return (0);
}
