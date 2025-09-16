/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:24:08 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/16 13:51:43 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_executor(t_ms *ms)
{
	ms->fd_in = -1;
	ms->fd_out = -1;
	ms->pids = NULL;
	ms->pid_index = 0;
	ms->cmd_path = NULL;
	ms->std_copy[0] = -1;
	ms->std_copy[1] = -1;
	ms->ms_fd[0] = -1;
	ms->ms_fd[1] = -1;
	ms->prev_fd = -1;
	ms->cmd_index = 0;
	ms->child = true;
	ms->reset[0] = 0;
	ms->reset[1] = 0;
}

void	wait_help(t_ms *ms)
{
	if (ms->pids[ms->pid_index] == -1)
		return ;
	waitpid(ms->pids[ms->pid_index], &ms->exit_status, 0);
	if (WIFEXITED(ms->exit_status))
		ms->exit_status = WEXITSTATUS(ms->exit_status);
	else if (WIFSIGNALED(ms->exit_status))
	{
		ms->exit_status = 128 + WTERMSIG(ms->exit_status);
		if (ms->exit_status == 131 && ms->pid_index == ms->cmd_no - 1)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		else if (ms->exit_status == 130 && ms->pid_index == ms->cmd_no - 1)
			ft_putstr_fd("\n", STDERR_FILENO);
	}
}

int	handle_files(t_ms *ms, t_cmd *cmd)
{
	if (cmd->infile && g_signal != SIGINT)
	{
		if (handle_infiles(ms, cmd->infile) < 0)
			return (1);
	}
	if (cmd->outfile && g_signal != SIGINT)
	{
		if (handle_outfiles(ms, cmd->outfile, cmd->append) < 0)
			return (1);
	}
	return (0);
}

void	run_executor(t_ms *ms)
{
	int	i;

	if (!ms->ast || ms->heredoc_stop)
		return ;
	init_executor(ms);
	ms->cmd_no = count_cmds(ms->ast);
	if (ms->cmd_no == 0)
		return ;
	ms->pids = malloc(ms->cmd_no * sizeof(pid_t));
	if (!ms->pids)
		ms_error(ms, "PID array malloc fail", 1, 0);
	i = -1;
	while (++i < ms->cmd_no)
		ms->pids[i] = -1;
	if (ms->cmd_no > 1)
	{
		if (pipe(ms->ms_fd) < 0)
			ms_error(ms, "Pipe faiure", 1, 0);
	}
	parse_cmds(ms, ms->ast);
	close_pipes(ms);
	ms->pid_index = -1;
	if (ms->child == true)
	{
		while (++ms->pid_index < ms->cmd_no)
			wait_help(ms);
		set_interactive_signals();
	}
	reset_std(ms);
	free(ms->pids);
	ms->pids = NULL;
}
