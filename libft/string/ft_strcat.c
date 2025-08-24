/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:04:47 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 03:05:05 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft.h"

char	*ft_strcat(char *dst, const char *src)
{
	int	i;
	int	dst_len;

	dst_len = ft_strlen(dst);
	i = 0;
	while (src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst);
}