/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* only_redir.c                                       :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:40:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 07:40:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 명령어 없이 리다이렉션만 있는 경우를 처리하는 자식 프로세스 로직.
 * 파일을 생성/오픈하고 즉시 종료합니다.
 */
void	run_redir(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0) // 자식 프로세스
	{
		reset_child_signals();
		if (handle_files(ms, ast->cmd)) // 파일 생성/오픈 시도
			bi_exit(ms, 1, 1);           // 실패 시 종료
		reset_std(ms);
		close_fd(ms);
		bi_exit(ms, 0, 1);           // 성공 시 0으로 종료
	}
	// 부모는 시그널 처리 로직이 필요하다면 여기에 추가 (minishell_team 참조)
}

/**
 * @brief 명령어 없는 리다이렉션의 실행을 시작하는 함수.
 * 파이프를 설정하고 자식 프로세스를 실행합니다.
 */
void	run_no_cmd(t_ms *ms, t_ast *ast)
{
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);
	run_redir(ms, ast);
	if (ms->cmd_no > 1) // 파이프라인의 일부인 경우
		next_pipe(ms, ms->cmd_index + 1 >= ms->cmd_no);
	reset_std(ms);
}
