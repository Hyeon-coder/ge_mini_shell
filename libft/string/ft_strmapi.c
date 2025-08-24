/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 20:10:28 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:50:53 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Applies a function to each character of a string and creates a new string.
	(문자열의 각 문자에 함수를 적용하여 새로운 문자열 생성)
*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*rst;
	unsigned int	len;
	unsigned int	i;

	if (!s || !f)
		return (0);
	len = ft_strlen(s);
	rst = (char *)malloc(len * sizeof(char) + 1);
	if (!rst)
		return (0);
	i = 0;
	while (s[i])
	{
		rst[i] = f(i, (char)s[i]);
		i++;
	}
	rst[i] = '\0';
	return (rst);
}
