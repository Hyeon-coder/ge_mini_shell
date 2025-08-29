/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* heredoc.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 03:20:00 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 09:45:00 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	close(STDIN_FILENO);
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
	char				*line;
	struct sigaction	sa_old;
	struct sigaction	sa_new;
	struct termios		original_termios;
	struct termios		new_termios;

	g_signal = 0;
	tcgetattr(STDIN_FILENO, &original_termios);
	new_termios = original_termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	ft_memset(&sa_new, 0, sizeof(sa_new));
	sa_new.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa_new, &sa_old);
	while (1)
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
	sigaction(SIGINT, &sa_old, NULL);
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
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
