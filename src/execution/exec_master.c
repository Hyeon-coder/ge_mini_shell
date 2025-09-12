/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* exec_master.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:10:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 07:10:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_ast *ast);
void		init_executor(t_ms *ms);
void		wait_help(t_ms *ms);

/**
 * @brief 재귀적으로 AST를 순회하여 실행할 명령어의 총 개수를 계산합니다.
 * @param ast 파싱된 AST의 현재 노드
 * @return 명령어의 총 개수
 */
static int	count_cmds(t_ast *ast)
{
	int	count;

	count = 0;
	if (!ast)
		return (0);
	if (ast->type == NODE_PIPE)
		count += count_cmds(ast->left) + count_cmds(ast->right);
	else
		count++;
	return (count);
}

/**
 * @brief 실행에 필요한 변수들을 초기화합니다.
 * 매 명령어 라인마다 호출되어 상태를 깨끗하게 리셋합니다.
 */
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

/**
 * @brief 자식 프로세스가 종료될 때까지 기다리고, 종료 코드를 올바르게 설정합니다.
 * 파이프라인의 마지막 명령어의 종료 코드가 최종 종료 코드가 됩니다.
 */
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

/**
 * @brief 실행부의 메인 진입 함수 (Parsing과의 인터페이스)
 * 이 함수는 자원 할당, 실행, 자원 해제의 명확한 라이프사이클을 관리합니다.
 */
void	run_executor(t_ms *ms)
{
	int	i;

	if (!ms->ast || ms->heredoc_stop)
		return ;
	// 1. 실행기 초기화
	init_executor(ms);
	ms->cmd_no = count_cmds(ms->ast);
	if (ms->cmd_no == 0)
		return ;
	// 2. [메모리 할당] 모든 자식 프로세스의 PID를 저장할 배열 할당
	ms->pids = malloc(ms->cmd_no * sizeof(pid_t));
	if (!ms->pids)
		ms_error(ms, "PID array malloc fail", 1, 0);
	i = -1;
	while (++i < ms->cmd_no)
		ms->pids[i] = -1; // PID 배열 초기화
	// 3. AST 순회 및 명령어 실행 시작 (다음 단계에서 구현할 함수)
	parse_cmds(ms, ms->ast);
	// 4. 열려 있는 모든 파일 디스크립터 정리
	close_fd(ms);
	// 5. 모든 자식 프로세스 대기
	ms->pid_index = -1;
	if (ms->child == true) // 단일 빌트인 명령이 아닐 경우에만 wait
	{
		while (++ms->pid_index < ms->cmd_no)
			wait_help(ms);
		set_interactive_signals();
	}
	// 6. [메모리 해제] 할당했던 PID 배열 해제
	free(ms->pids);
	ms->pids = NULL;
}
