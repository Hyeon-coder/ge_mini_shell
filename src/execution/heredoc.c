/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:45:28 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/30 22:14:25 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	heredoc_child_loop(t_ms *ms, char *lim, int fd, int quo)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(ms, line, fd, (quo == UNQUOTED));
	}
	exit(0);
}

/*
** The parent process waits for the heredoc child process and handles the result.
** It checks if the child was terminated by a signal and updates the shell state.
*/
static int	handle_heredoc_parent(t_ms *ms, pid_t pid,\
									char *filename, t_infile *infile)
{
	int	status;

	waitpid(pid, &status, 0);
	set_interactive_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		ms->heredoc_stop = true;
		ms->exit_status = 1;
		write(1, "\n", 1);
		unlink(filename);
		free(filename);
		return (0);
	}
	free(infile->name);
	infile->name = filename;
	return (0);
}

/*
** Initializes the heredoc process. It creates a temporary file,
** forks a child process to handle user input, and waits for its completion.
*/
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	int		fd;
	char	*filename;
	pid_t	pid;

	fd = open_heredoc_file(&filename, ms);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		ms_error(ms, "fork failed", 1, 0);
	if (pid == 0)
		heredoc_child_loop(ms, lim, fd, quo);
	close(fd);
	return (handle_heredoc_parent(ms, pid, filename, infile));
}
