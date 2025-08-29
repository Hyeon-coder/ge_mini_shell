/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:23:50 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 14:23:54 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	rl_done = 1;
	close(STDIN_FILENO);
}

char	*generate_heredoc_filename(t_ms *ms)
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

void	setup_heredoc_signals(void)
{
	struct sigaction	sa_new;
	struct termios		new_termios;

	g_signal = 0;
	rl_done = 0;
	tcgetattr(STDIN_FILENO, &g_original_termios);
	new_termios = g_original_termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	ft_memset(&sa_new, 0, sizeof(sa_new));
	sa_new.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa_new, &g_sa_old);
}

void	restore_heredoc_signals(void)
{
	sigaction(SIGINT, &g_sa_old, NULL);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_original_termios);
}
