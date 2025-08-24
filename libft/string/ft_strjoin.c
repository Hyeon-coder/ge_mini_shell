/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:39:38 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:33:10 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Concatenates two strings. (두 문자열을 이어 붙임)
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	i;
	char	*rst;

	len = ft_strlen(s1);
	rst = (char *)malloc(sizeof(char) * (len + ft_strlen(s2) + 1));
	if (!rst)
		return (NULL);
	if (s1 || s2)
	{
		i = -1;
		while (s1[++i])
			rst[i] = s1[i];
		i = -1;
		while (s2[++i])
		{
			rst[len] = s2[i];
			len++;
		}
		rst[len] = '\0';
	}
	else
		return (NULL);
	return (rst);
}
