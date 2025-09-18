/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:24:08 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/18 15:25:38 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Initializes all variables for the executor before running commands.
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

// Waits for a single child process and handles its exit status.
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

// Sets up file redirections for a command.
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

// Orchestrates the entire command execution process, from setup to cleanup.
void	run_executor(t_ms *ms)
{
	if (!ms->ast || ms->heredoc_stop)
		return ;
	init_executor(ms);
	ms->cmd_no = count_cmds(ms->ast);
	if (ms->cmd_no == 0)
		return ;
	prepare_executor_resources(ms);
	set_noninteractive_signals();
	parse_cmds(ms, ms->ast);
	close_pipes(ms);
	wait_for_processes(ms);
	finalize_execution(ms);
}
