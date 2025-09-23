/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:11:24 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/23 13:21:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints an error and exits if dup fails.
void	dup_fail(t_ms *ms, int i)
{
	if (ms->std_copy[i] < 0)
		ms_error(ms, "stdout duplication failed", 1, 0);
}

// Prints an error and exits if dup2 fails.
void	redi_fail(t_ms *ms, bool is_out)
{
	if (is_out == true)
	{
		if (dup2(ms->fd_out, STDOUT_FILENO) < 0)
			ms_error(ms, "stdout redirection failed", 1, 0);
	}
	else
	{
		if (dup2(ms->fd_in, STDIN_FILENO) < 0)
			ms_error(ms, "stdin redirection failed", 1, 0);
	}
}

// Processes a single input file, handling its redirection.
int	process_single_infile(t_ms *ms, t_infile *infile)
{
	ms->fd_in = open(infile->name, O_RDONLY);
	if (ms->fd_in < 0)
	{
		perror(infile->name);
		reset_std(ms);
		return (-1);
	}
	if (infile->heredoc == 1)
	{
		unlink(infile->name);
		free(infile->name);
		infile->name = NULL;
	}
	redi_fail(ms, false);
	close(ms->fd_in);
	ms->fd_in = -1;
	return (0);
}

// Reads user input for a here-document until a limiter is found.
void	heredoc_input_loop(t_ms *ms, int fd, const char *limiter, int quoted)
{
	char	*line;
	char	*expanded_line;
	bool	was_expanded;

	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT || !line || ft_strcmp(line, limiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		if (quoted == UNQUOTED)
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
}

// Closes the here-document file descriptor and restores signals handlers.
int	cleanup_heredoc(t_ms *ms, int fd)
{
	close(fd);
	rl_event_hook = NULL;
	set_interactive_signals();
	if (g_signal == SIGINT)
	{
		ms->heredoc_stop = true;
		g_signal = 0;
		return (-1);
	}
	return (0);
}
