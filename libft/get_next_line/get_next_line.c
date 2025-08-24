/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:17:39 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/24 22:01:09 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
# include <stdio.h>

char	*get_before_newline(const char *s)
{
	char	*res;
	int		i;

	i = 0;
	while (s[i] != '\0' && s[i] != '\n')
		i++;
	if (s[i] != '\0' && s[i] == '\n')
		i++;
	res = ft_malloc_zero(i + 1, sizeof * res);
	if (!res)
		return (NULL);
	i = 0;
	while (s[i] != '\0' && s[i] != '\n')
	{
		res[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
	{
		res[i] = s[i];
		i++;
	}
	return (res);
}

char	*get_after_newline(const char *s)
{
	char	*res;
	int		i;
	int		j;

	j = 0;
	while (s && s[j])
		j++;
	i = 0;
	while (s[i] != '\0' && s[i] != '\n')
		i++;
	if (s[i] != '\0' && s[i] == '\n')
		i++;
	res = ft_malloc_zero((j - i) + 1, sizeof * res);
	if (!res)
		return (NULL);
	j = 0;
	while (s[i + j])
	{
		res[j] = s[i + j];
		j++;
	}
	return (res);
}

void	ft_read_line(int fd, char **keep, char **tmp)
{
	char	*buf;
	int		r;

	buf = malloc(sizeof * buf * (BUFFER_SIZE + 1));
	if (!buf)
		return ;
	r = 1;
	while (r > 0)
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r == -1)
		{
			ft_free_strs(&buf, keep, tmp);
			return ;
		}
		buf[r] = '\0';
		*tmp = ft_strdup(*keep);
		ft_free_strs(keep, 0, 0);
		*keep = join_strs(*tmp, buf);
		ft_free_strs(tmp, 0, 0);
		if (contains_newline(*keep))
			break ;
	}
	ft_free_strs(&buf, 0, 0);
}

// 기존 ft_parse_line 함수를 아래 내용으로 완전히 교체합니다.
char	*ft_parse_line(char **keep)
{
	char	*line;
	char	*new_keep;
	char	*newline_ptr;
	size_t	line_len;

	newline_ptr = ft_strchr(*keep, '\n');
	if (newline_ptr)
	{
		// 줄바꿈 문자를 찾았을 경우
		line_len = (newline_ptr - *keep) + 1;
		line = ft_substr(*keep, 0, line_len);
		if ((*keep)[line_len] != '\0')
			new_keep = ft_strdup(&(*keep)[line_len]);
		else
			new_keep = NULL;
	}
	else
	{
		// 줄바꿈 문자가 없고 파일 끝(EOF)인 경우
		line = ft_strdup(*keep);
		new_keep = NULL;
	}
	free(*keep); // 이전 keep 메모리를 여기서 안전하게 해제
	*keep = new_keep; // keep을 나머지 문자열로 업데이트
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*keep = NULL;
	char		*buf;
	char		*temp;
	int			bytes_read;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	line = NULL;
	bytes_read = 1;
	// keep에 줄바꿈이 없을 때까지 파일을 읽어서 keep에 추가
	while (bytes_read > 0 && !ft_strchr(keep, '\n'))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read <= 0)
			break;
		buf[bytes_read] = '\0';
		temp = ft_strjoin(keep, buf);
		free(keep);
		keep = temp;
	}
	free(buf);
	if (bytes_read < 0) // read 에러 처리
	{
		free(keep);
		keep = NULL;
		return (NULL);
	}
	if (keep && *keep) // keep에 내용이 남아있으면 line으로 추출
		line = ft_parse_line(&keep);
	if (!line || *line == '\0') // 추출된 라인이 비어있으면 NULL 반환
	{
		free(line);
		return (NULL);
	}
	return (line);
}
