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
	// 이 함수는 parse_cmds에서 재귀적으로 호출되며, 
	// 각 명령어 노드에 대해 적절한 exec_*_pipe 함수를 호출해야 합니다.
	// 실제 구현에서는 minishell_team의 parse_cmds와 유사한 로직으로
	// ast->left, ast->right를 탐색하며 순서에 맞게 호출해야 합니다.
	// 여기서는 개념적인 예시를 보입니다.
	(void)ast; 
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);

	// 예시: ms->ast를 순회하며 순서대로 exec_first, mid, last를 호출한다고 가정
	// exec_first_pipe(ms, first_node);
	// next_pipe(ms, 0);
	// exec_mid_pipe(ms, mid_node);
	// next_pipe(ms, 0);
	// exec_last_pipe(ms, last_node);

	close_pipes(ms);
}
