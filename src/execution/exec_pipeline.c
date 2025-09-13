/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* exec_pipeline.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:25:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 10:15:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

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
		if (dup2(ms->ms_fd[1], STDOUT_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		// [수정] 자식은 더 이상 사용하지 않는 파이프 FD를 모두 닫는다.
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

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
		if (dup2(ms->prev_fd, STDIN_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		if (dup2(ms->ms_fd[1], STDOUT_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		// [수정] 자식은 더 이상 사용하지 않는 파이프 FD를 모두 닫는다.
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

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
		if (dup2(ms->prev_fd, STDIN_FILENO) < 0)
			ms_error(ms, "dup2 failed", 1, 1);
		// [수정] 자식은 더 이상 사용하지 않는 파이프 FD를 모두 닫는다.
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

void	run_pipes(t_ms *ms, t_ast *ast)
{
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);
	parse_cmds(ms, ast->left);
	next_pipe(ms, (ms->cmd_index + 1) >= ms->cmd_no);
	parse_cmds(ms, ast->right);
}
