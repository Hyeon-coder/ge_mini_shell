/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:37:13 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/16 13:37:39 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

int	heredoc_rl_event_hook(void)
{
	if (g_signal == SIGINT)
		rl_done = 1;
	return (0);
}
