/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* heredoc.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 17:53:24 by JuHyeon           #+#    #+#             */
/* Updated: 2025/08/30/ 04:00:00 by JuHyeon          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/*
** This is the signal handler for SIGINT (Ctrl+C) specifically
** during heredoc input. It sets a global flag and uses ioctl to push a
** newline character into the input buffer. This forces the blocked
** readline() call to return immediately, allowing the loop to terminate.
*/
void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

/*
** Writes a single line to the heredoc's temporary file.
** It expands variables within the line if the delimiter was not quoted.
*/
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

/*
** The main loop for reading heredoc input from the user. It sets up
** temporary signal and terminal settings to handle Ctrl+C interruptions
** correctly, mimicking the behavior of bash.
*/
static int	heredoc_loop(t_ms *ms, char *lim, int fd, int quo)
{
	char				*line;
	struct sigaction	sa_old;
	struct termios		orig_termios;

	setup_heredoc_handlers(&orig_termios, &sa_old);
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
	restore_handlers(&orig_termios, &sa_old);
	if (g_signal == SIGINT)
	{
		ms->exit_status = 1;
		ms->heredoc_stop = true;
	}
	return (0);
}

/*
** Initializes the heredoc process. It creates a temporary file,
** runs the input loop, and finalizes the redirection by updating
** the infile information for the command.
*/
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	int		fd;
	char	*filename;
	int		stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
		ms_error(ms, "dup failed", 1, 0);
	fd = open_heredoc_file(&filename, ms);
	heredoc_loop(ms, lim, fd, quo);
	close(fd);
	return (finalize_heredoc(ms, infile, filename, stdin_copy));
}
