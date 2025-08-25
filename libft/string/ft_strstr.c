/* ***************************************************************************/
/*                                                                           */
/*                                                         :::      :::::::: */
/*    ft_strstr.c                                        :+:      :+:    :+: */
/*                                                     +:+ +:+         +:+   */
/*    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+      */
/*                                                 +#+#+#+#+#+   +#+         */
/*    Created: 2025/08/25 10:46:48 by juhyeonl          #+#    #+#           */
/*    Updated: 2025/08/25 10:46:54 by juhyeonl         ###   ########.fr     */
/*                                                                           */
/* ***************************************************************************/

# include "../libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;
	size_t	needle_len;

	if (!haystack || !needle)
		return (NULL);
	if (!*needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i + j] && needle[j] && haystack[i + j] == needle[j])
			j++;
		if (j == needle_len)
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}
