/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* exec_pipeline.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13/ 07:25:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13/ 07:25:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 현재 파이프를 정리하고 다음 명령어를 위해 새 파이프를 생성합니다.
 * @param last 파이프라인의 마지막 명령어인지 여부
 */
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

/**
 * @brief 파이프라인의 첫 번째 명령어를 실행합니다.
 */
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
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

/**
 * @brief 파이프라인의 중간 명령어를 실행합니다.
 */
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
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

/**
 * @brief 파이프라인의 마지막 명령어를 실행합니다.
 */
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
		close_pipes(ms);
		run_cmd(ms, ast->cmd);
	}
}

/**
 * @brief 파이프라인 실행을 총괄합니다.
 */
void	run_pipes(t_ms *ms, t_ast *ast)
{
	// 파이프 생성
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);

	// AST의 왼쪽 자식 노드 (파이프의 왼쪽에 있는 명령어)를 먼저 처리
	// 이 노드가 또 다른 파이프일 수 있으므로 parse_cmds를 재귀 호출
	parse_cmds(ms, ast->left);

	// 파이프 정리 및 다음 파이프 준비
	next_pipe(ms, (ms->cmd_index + 1) >= ms->cmd_no);

	// AST의 오른쪽 자식 노드 (파이프의 오른쪽에 있는 명령어)를 처리
	parse_cmds(ms, ast->right);
}
