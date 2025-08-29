/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* heredoc.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/* mpllecommeentaire*/
/* Created: 2025/08/29 17:53:24 by JuHyeon           #+#    #+#             */
/* Updated: 2025/08/30 02:10:00 by JuHyeon          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

// An event hook function for readline.
// It is called periodically while readline is waiting for input.
// Returns 1 to terminate readline if a SIGINT signal has been caught.
static int	heredoc_event_hook(void)
{
	if (g_signal == SIGINT)
		return (1);
	return (0);
}

// Signal handler for SIGINT during heredoc input.
// This function is kept simple and only sets a global flag,
// which is a safe operation within a signal handler.
void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
}

// Writes a single line to the heredoc's temporary file.
// It expands variables within the line if it was not quoted.
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

// The main loop for reading heredoc input from the user.
// It sets up a readline event hook to immediately catch Ctrl+C interruptions
// without requiring an extra key press.
static int	heredoc_loop(t_ms *ms, char *lim, int fd, int quo)
{
	char				*line;
	struct sigaction	sa_old;
	struct termios		orig_termios;

	setup_heredoc_handlers(&orig_termios, &sa_old);
	rl_event_hook = heredoc_event_hook;
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT || !line || ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(ms, line, fd, (quo == UNQUOTED));
	}
	rl_event_hook = NULL;
	restore_handlers(&orig_termios, &sa_old);
	if (g_signal == SIGINT)
	{
		ms->exit_status = 1;
		ms->heredoc_stop = true;
	}
	return (0);
}

// Initializes the heredoc process. It creates a temporary file,
// runs the input loop, and finalizes the redirection.
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	int		fd;
	char	*filename;
	int		stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	fd = open_heredoc_file(&filename, ms);
	heredoc_loop(ms, lim, fd, quo);
	close(fd);
	return (finalize_heredoc(ms, infile, filename, stdin_copy));
}
