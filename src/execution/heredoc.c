/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:23:22 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 14:23:24 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_to_heredoc(t_ms *ms, char *line, int fd, int expand)
{
	char	*expanded_line;
	bool	was_expanded;

	if (expand)
	{
		was_expanded = false;
		expanded_line = expand_input(ms, line, &was_expanded);
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

	setup_heredoc_signals();
	while (!rl_done)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (g_signal == SIGINT || ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(ms, line, fd, (quo == UNQUOTED));
	}
	restore_heredoc_signals();
	if (g_signal == SIGINT)
	{
		ms->exit_status = 1;
		ms->heredoc_stop = true;
	}
	return (0);
}

int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	int		fd;
	char	*filename;
	int		stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	filename = generate_heredoc_filename(ms);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		ms_error(ms, "open failed for heredoc", 1, 0);
		close(stdin_copy);
		return (1);
	}
	heredoc_loop(ms, lim, fd, quo);
	close(fd);
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	if (ms->heredoc_stop)
	{
		unlink(filename);
		free(filename);
		return (0);
	}
	free(infile->name);
	infile->name = filename;
	return (0);
}
