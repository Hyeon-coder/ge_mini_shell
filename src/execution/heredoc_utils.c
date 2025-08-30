/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:38:08 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/30 21:45:01 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets up the terminal and signal handlers for here-document input.
 * It saves the original terminal attributes and sets a custom SIGINT handler
 * to manage interruptions gracefully. It also disables the echoing of
 * control characters like ^C.
 */
void	setup_heredoc_handlers(struct termios *orig_termios,
								struct sigaction *sa_old)
{
	struct sigaction	sa_new;
	struct termios		new_termios;

	g_signal = 0;
	tcgetattr(STDIN_FILENO, orig_termios);
	new_termios = *orig_termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	ft_memset(&sa_new, 0, sizeof(sa_new));
	sa_new.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, sa_old);
}

/**
 * @brief Restores the original terminal and signal handler settings after
 * the here-document input loop is finished.
 */
void	restore_handlers(struct termios *orig_termios, struct sigaction *sa_old)
{
	sigaction(SIGINT, sa_old, NULL);
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

static char	*generate_heredoc_filename(t_ms *ms)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(ms->heredoc_no++);
	if (!num_str)
		ms_error(ms, "ft_itoa failed in heredoc", 1, 0);
	filename = ft_strjoin(".heredoc_", num_str);
	free(num_str);
	if (!filename)
		ms_error(ms, "ft_strjoin failed in heredoc", 1, 0);
	return (filename);
}

/**
 * Creates and opens a temporary file for the here-document. It generates a
 * unique filename and handles errors if the file creation fails.
 */
int	open_heredoc_file(char **filename, t_ms *ms)
{
	int	fd;

	*filename = generate_heredoc_filename(ms);
	fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(*filename);
		ms_error(ms, "open failed for heredoc", 1, 0);
	}
	return (fd);
}

/**
 * Finalizes the here-document process after the input loop. This function
 * restores standard input and cleans up the temporary file if the process
 * was interrupted. Otherwise, it assigns the filename to the command's infile.
 */
int	finalize_heredoc(t_ms *ms, t_infile *infile, char *filename, int stdin_copy)
{
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
