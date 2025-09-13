/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* exec_cmds.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13/ 07:15:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13/ 07:15:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 명령어가 빌트인 명령어인지 확인합니다.
 * @param cmd 확인할 커맨드 구조체
 * @return 빌트인이면 1, 아니면 0을 반환합니다.
 */
int	check_for_bi(t_cmd *cmd)
{
	if (!cmd || !cmd->full_cmd || !cmd->full_cmd[0])
		return (0);
	if (!ft_strcmp(cmd->full_cmd[0], "cd") || \
		!ft_strcmp(cmd->full_cmd[0], "env") || \
		!ft_strcmp(cmd->full_cmd[0], "export") || \
		!ft_strcmp(cmd->full_cmd[0], "unset") || \
		!ft_strcmp(cmd->full_cmd[0], "echo") || \
		!ft_strcmp(cmd->full_cmd[0], "pwd") || \
		!ft_strcmp(cmd->full_cmd[0], "exit"))
		return (1);
	return (0);
}

/**
 * @brief 빌트인 또는 외부 명령어를 최종적으로 실행합니다.
 * @param ms 메인 구조체
 * @param cmd 실행할 커맨드 구조체
 */
void	run_cmd(t_ms *ms, t_cmd *cmd)
{
	if (check_for_bi(cmd))
	{
		if (!ft_strcmp(cmd->full_cmd[0], "echo"))
			builtin_echo(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "cd"))
			builtin_cd(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "pwd"))
			builtin_pwd(ms);
		else if (!ft_strcmp(cmd->full_cmd[0], "export"))
			builtin_export(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "unset"))
			builtin_unset(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "env"))
			builtin_env(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "exit"))
			builtin_exit(ms, cmd);
	}
	else
		run_execve(ms, cmd);
	if (ms->child == true)
		bi_exit(ms, ms->exit_status, 1);
}

/**
 * @brief 단일 명령어(파이프 없는)를 처리합니다.
 * - 빌트인: 부모 프로세스에서 직접 실행 (cd, export 등이 셸 자체에 영향을 줘야 하므로)
 * - 외부 명령어: 자식 프로세스를 생성하여 실행
 */
void	run_one(t_ms *ms, t_cmd *cmd)
{
	if (check_for_bi(cmd))
	{
		ms->child = false; // 부모 프로세스에서 실행함을 명시
		if (handle_files(ms, cmd)) // 리다이렉션 처리 (3단계에서 구현)
			return ;
		if (g_signal == SIGINT) // heredoc 중단 시 실행 안 함
			return ;
		run_cmd(ms, cmd);
		return ;
	}
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0) // 자식 프로세스
	{
		reset_child_signals();
		if (handle_files(ms, cmd) || g_signal) // 리다이렉션 또는 heredoc 중단
			bi_exit(ms, 1, 1);
		run_cmd(ms, cmd);
	}
	// 부모 프로세스는 자식이 끝날 때까지 시그널을 무시하도록 설정할 수 있습니다.
	// hold_signals(); 
}

/**
 * @brief 명령어 실행 흐름을 파이프라인 여부/위치에 따라 분기합니다.
 */
void	start_cmds(t_ms *ms, t_ast *ast)
{
	if (ms->cmd_no == 1) // 단일 명령어인 경우
		run_one(ms, ast->cmd);
	else // 파이프라인의 일부인 경우
	{
		if (ms->cmd_index == 1) // 첫 번째 명령어
			exec_first_pipe(ms, ast);
		else if (ms->cmd_index == ms->cmd_no) // 마지막 명령어
			exec_last_pipe(ms, ast);
		else // 중간 명령어
			exec_mid_pipe(ms, ast);
	}
	// reset_std는 run_executor의 루프가 끝난 후 한 번만 호출되므로 여기서 제거합니다.
}

/**
 * @brief AST를 재귀적으로 순회하며 실행 순서를 제어하는 최종 "엔진"
 */
void	parse_cmds(t_ms *ms, t_ast *ast)
{
	if (!ast || g_signal == SIGINT)
		return ;
	if (ast->type == NODE_PIPE)
	{
		// [수정] 파이프를 여기서 생성합니다.
		// if (pipe(ms->ms_fd) < 0)
		// 	ms_error(ms, "Pipe failure", 1, 0);
		
		// 왼쪽 자식(먼저 실행될 명령어)을 재귀 호출
		parse_cmds(ms, ast->left);
		
		// [수정] 파이프의 FD를 다음 명령어를 위해 정리 및 교체
		next_pipe(ms, (ms->cmd_index + 1) >= ms->cmd_no);
		
		// 오른쪽 자식(나중에 실행될 명령어)을 재귀 호출
		parse_cmds(ms, ast->right);
	}
	else if (ast->type == NODE_COMMAND || ast->type == NODE_MISSCMD)
	{
		ms->cmd_index++;
		if (ast->cmd && ast->cmd->full_cmd)
			start_cmds(ms, ast);
		else
			run_no_cmd(ms, ast);
	}
}
