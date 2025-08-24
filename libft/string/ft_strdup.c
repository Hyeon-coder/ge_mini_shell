/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:49:52 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:50:26 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Duplicates a string. (문자열 복사)
*/
char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s1) + 1;
	dup = (char *)malloc(len);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s1, len);
	return (dup);
}
