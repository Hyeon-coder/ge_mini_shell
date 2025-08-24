/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ft_strjoin_helpers.c                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "libft.h"

// s1과 s2를 합친 후, s1의 메모리를 해제합니다.
char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*new_str;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

// 세 개의 문자열을 순서대로 합칩니다.
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

// dest 문자열에 src의 [start, end) 부분을 잘라 이어붙입니다.
void	append_substr(char **dest, char const *src, int start, int end)
{
	char	*substr;
	char	*new_dest;

	if (start >= end)
		return ;
	substr = ft_substr(src, start, end - start);
	if (!substr)
		return ;
	new_dest = ft_strjoin(*dest, substr);
	free(substr);
	free(*dest);
	*dest = new_dest;
}
