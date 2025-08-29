/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* heredoc.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 03:20:00 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 03:45:00 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static char	*generate_heredoc_filename(t_ms *ms)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(ms->heredoc_no);
	if (!num_str)
		ms_error(ms, "ft_itoa failed in heredoc", 1, 0);
	filename = ft_strjoin(".heredoc_", num_str);
	free(num_str);
	if (!filename)
		ms_error(ms, "ft_strjoin failed in heredoc", 1, 0);
	ms->heredoc_no++;
	return (filename);
}

static void	write_to_heredoc(t_ms *ms, char *line, int fd, int expand)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_input(ms, line, NULL);
		ft_putendl_fd(expanded_line, fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, fd);
	free(line);
}

static int	heredoc_loop(t_ms *ms, char *lim, int fd, int quo)
{
	char	*line;
	bool	should_expand;

	should_expand = (quo == UNQUOTED);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(ms, line, fd, should_expand);
	}
	return (0);
}

int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	int		fd;
	char	*filename;

	filename = generate_heredoc_filename(ms);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		ms_error(ms, "open failed for heredoc", 1, 0);
		return (1);
	}
	if (heredoc_loop(ms, lim, fd, quo))
	{
		close(fd);
		unlink(filename);
		free(filename);
		return (1);
	}
	close(fd);
	free(infile->name);
	infile->name = filename;
	return (0);
}
