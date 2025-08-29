/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:38:08 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 18:01:49 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
