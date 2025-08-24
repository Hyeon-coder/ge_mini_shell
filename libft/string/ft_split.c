/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 12:38:12 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:35:32 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Frees allocated memory for a split string. (분할된 문자열 메모리 해제)
*/
static void	free_arr(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

/*
	Counts the number of delimiters in a string. (문자열에서 구분자 개수 계산)
*/
static int	cnt_deli(char const *s, char c)
{
	int	cnt;

	if (!s || !*s)
		return (0);
	cnt = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
			cnt++;
		while (*s && *s != c)
			s++;
	}
	return (cnt);
}

/*
	Performs string splitting based on delimiters. (구분자를 기준으로 문자열을 분할)
*/
static char	*get_word(const char **s, char c)
{
	size_t		len;
	char		*word;
	const char	*end;

	while (**s && **s == c)
		(*s)++;
	end = ft_strchr(*s, c);
	if (end)
		len = end - *s;
	else
		len = ft_strlen(*s);
	word = ft_substr(*s, 0, len);
	*s += len;
	return (word);
}

/*
	Returns a string into an array, separated by a separator.
	(문자열을 구분자로 나누어 배열로 반환)
*/
char	**ft_split(char const *s, char c)
{
	int		i;
	char	**arr;

	if (!s)
		return (NULL);
	arr = (char **)malloc((cnt_deli(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		arr[i] = get_word(&s, c);
		if (!arr[i])
		{
			free_arr(arr, i - 1);
			return (NULL);
		}
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
