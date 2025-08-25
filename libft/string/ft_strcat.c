
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
