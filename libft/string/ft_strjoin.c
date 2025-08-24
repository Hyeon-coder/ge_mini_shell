/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ft_strjoin.c                                       :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2024/11/01 16:39:38 by JuHyeon           #+#    #+#             */
/* Updated: 2025/08/24 20:30:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../libft.h"

/*
	Concatenates two strings. (두 문자열을 이어 붙임)
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_str = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!new_str)
		return (NULL);
	if (s1)
		ft_memcpy(new_str, s1, s1_len);
	if (s2)
		ft_memcpy(new_str + s1_len, s2, s2_len);
	new_str[s1_len + s2_len] = '\0';
	return (new_str);
}
