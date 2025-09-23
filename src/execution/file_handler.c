/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:24:56 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/23 13:22:08 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Generates a unique temporary filename a here-document.
char	*heredoc_name(t_ms *ms, int i)
{
	char	*name;
	char	*no;

	no = ft_itoa(i);
	if (!no)
		ms_error(ms, "ft_itoa failure in heredoc", 1, 0);
	name = ft_strjoin(".heredoc", no);
	free(no);
	if (!name)
		ms_error(ms, "heredoc file generation failure", 1, 0);
	return (name);
}

// Creates a temporary file and handles user input for a here-document.
int	handle_heredoc(t_ms *ms, const char *limiter, char *name, int quoted)
{
	int	fd;

	signal(SIGINT, heredoc_sigint_handler);
	rl_event_hook = heredoc_rl_event_hook;
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_error(ms, "Failed to create temporary file for HEREDOC", 1, 0);
	heredoc_input_loop(ms, fd, limiter, quoted);
	return (cleanup_heredoc(ms, fd));
}

// Starts the here-document process and assigns the temporary filename.
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	char	*temp_filename;

	temp_filename = heredoc_name(ms, ms->heredoc_no - 1);
	if (handle_heredoc(ms, lim, temp_filename, quo) == -1)
	{
		unlink(temp_filename);
		free(temp_filename);
		ms->heredoc_no--;
		return (1);
	}
	if (infile->name)
		free(infile->name);
	infile->name = temp_filename;
	return (0);
}

// Processes a list of input files for redirection.
int	handle_infiles(t_ms *ms, t_infile **infile)
{
	int	i;

	i = -1;
	ms->std_copy[0] = dup(STDIN_FILENO);
	dup_fail(ms, 0);
	ms->reset[0] = 1;
	while (infile[++i])
	{
		if (process_single_infile(ms, infile[i]) != 0)
			return (-1);
	}
	return (0);
}

// Processes a list of output files for redirection.
int	handle_outfiles(t_ms *ms, char **outfile, int *append)
{
	int	i;
	int	open_flags;

	i = -1;
	ms->std_copy[1] = dup(STDOUT_FILENO);
	dup_fail(ms, 1);
	ms->reset[1] = 1;
	while (outfile[++i])
	{
		if (append[i] == 1)
			open_flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		ms->fd_out = open(outfile[i], open_flags, 0644);
		if (ms->fd_out < 0)
		{
			perror(outfile[i]);
			reset_std(ms);
			return (-1);
		}
		redi_fail(ms, true);
		close(ms->fd_out);
		ms->fd_out = -1;
	}
	return (0);
}
