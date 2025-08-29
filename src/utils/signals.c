/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* signals.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/17 13:50:02 by mpierce           #+#    #+#             */
/* Updated: 2025/08/29 06:40:02 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	do_sigint(int a, siginfo_t *b, void *c)
{
	(void)a;
	(void)b;
	(void)c;
	get_minishell(NULL)->exit_status = 1;
	g_signal = 0;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_interactive_signals(void)
{
	struct sigaction	act;
	struct sigaction	sigint;

	ft_memset(&act, 0, sizeof(act));
	ft_memset(&sigint, 0, sizeof(sigint));
	act.sa_handler = SIG_IGN;
	sigint.sa_sigaction = do_sigint;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &sigint, NULL);
}

void	set_noninteractive_signals(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &act, NULL);
}

void	reset_child_signals(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &act, NULL);
}

t_ms	*get_minishell(t_ms *ms)
{
	static t_ms	*minishell;

	if (ms)
	{
		minishell = ms;
		return (NULL);
	}
	return (minishell);
}
