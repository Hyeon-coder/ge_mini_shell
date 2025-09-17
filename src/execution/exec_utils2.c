/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:02:05 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/18 00:33:35 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Allocates resources like PID arrays and pipes before execution.
void	prepare_executor_resources(t_ms *ms)
{
	int	i;

	ms->pids = malloc(ms->cmd_no * sizeof(pid_t));
	if (!ms->pids)
		ms_error(ms, "PID array malloc fail", 1, 0);
	i = -1;
	while (++i < ms->cmd_no)
		ms->pids[i] = -1;
	if (ms->cmd_no > 1)
	{
		if (pipe(ms->ms_fd) < 0)
			ms_error(ms, "Pipe failure", 1, 0);
	}
}

// Waits for child processes to finish.
void	wait_for_processes(t_ms *ms)
{
	if (ms->child == true)
	{
		ms->pid_index = -1;
		while (++ms->pid_index < ms->cmd_no)
			wait_help(ms);
		set_interactive_signals();
	}
}

// Performs final cleanup of resources after all commands are executed.
void	finalize_execution(t_ms *ms)
{
	reset_std(ms);
	free(ms->pids);
	ms->pids = NULL;
}
