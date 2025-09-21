/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:24:43 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/22 02:18:14 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Sets up pipes for the next command in the pipeline.
void	next_pipe(t_ms *ms, int last)
{
	if (ms->prev_fd != -1)
		close(ms->prev_fd);
	close(ms->ms_fd[1]);
	ms->ms_fd[1] = -1;
	ms->prev_fd = ms->ms_fd[0];
	ms->ms_fd[0] = -1;
	if (last)
		return ;
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);
}

// Executes the first command in a pipeline.
void	exec_first_pipe(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0)
	{
		reset_child_signals();
		if (handle_files(ms, ast->cmd) < 0)
			bi_exit(ms, 1, 1);
		close(ms->ms_fd[0]);
		if (dup2(ms->ms_fd[1], STDOUT_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

// Executes a command in the middle of a pipeline.
void	exec_mid_pipe(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0)
	{
		reset_child_signals();
		if (handle_files(ms, ast->cmd) < 0)
			bi_exit(ms, 1, 1);
		close(ms->ms_fd[0]);
		close(ms->prev_fd);
		if (dup2(ms->prev_fd, STDIN_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		if (dup2(ms->ms_fd[1], STDOUT_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

// Executes the last command in a pipeline.
void	exec_last_pipe(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0)
	{
		reset_child_signals();
		if (handle_files(ms, ast->cmd) < 0)
			bi_exit(ms, 1, 1);
		close(ms->ms_fd[0]);
		close(ms->ms_fd[1]);
		if (dup2(ms->prev_fd, STDIN_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 0);
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

// Sets up the pipeline and dispatches commands for execution.
void	run_pipes(t_ms *ms, t_ast *ast)
{
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);
	parse_cmds(ms, ast->left);
	next_pipe(ms, (ms->cmd_index + 1) >= ms->cmd_no);
	parse_cmds(ms, ast->right);
}
