/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:44:59 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 10:45:04 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Extracts a line from the buffer ('keep') and updates the buffer
** to contain the remaining characters.
*/
static char	*ft_parse_line(char **keep)
{
	char	*line;
	char	*new_keep;
	char	*newline_ptr;
	size_t	line_len;

	if (!*keep || !**keep)
		return (NULL);
	newline_ptr = ft_strchr(*keep, '\n');
	if (newline_ptr)
	{
		line_len = (newline_ptr - *keep) + 1;
		line = ft_substr(*keep, 0, line_len);
		if ((*keep)[line_len] != '\0')
			new_keep = ft_strdup(&(*keep)[line_len]);
		else
			new_keep = NULL;
	}
	else
	{
		line = ft_strdup(*keep);
		new_keep = NULL;
	}
	free(*keep);
	*keep = new_keep;
	return (line);
}

/*
** Reads from the file descriptor and appends the content to the buffer ('keep')
** until a newline character is found or the end of the file is reached.
*/
static int	ft_read_to_keep(int fd, char **keep)
{
	char	*buf;
	char	*temp;
	int		bytes_read;

	buf = (char *)malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!buf)
		return (-1);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buf[bytes_read] = '\0';
		if (!*keep)
			temp = ft_strdup(buf);
		else
			temp = ft_strjoin(*keep, buf);
		free(*keep);
		*keep = temp;
		if (ft_strchr(*keep, '\n'))
			break ;
	}
	free(buf);
	return (bytes_read);
}

/*
** Reads a line from a file descriptor.
** Subsequent calls read the following lines.
*/
char	*get_next_line(int fd)
{
	static char	*keep = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (ft_read_to_keep(fd, &keep) < 0)
	{
		free(keep);
		keep = NULL;
		return (NULL);
	}
	line = ft_parse_line(&keep);
	if (!line || *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
