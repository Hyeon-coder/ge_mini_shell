/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* error.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:35:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 07:35:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 에러 메시지를 출력하고, 필요시 할당된 메시지를 해제한 후,
 * 모든 자원을 정리하고 주어진 종료 코드로 셸을 종료합니다.
 * 이 함수는 프로젝트 전역에서 사용되는 중앙 에러 처리기입니다.
 */
void	ms_error(t_ms *ms, char *msg, int ex_code, int free_msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		ft_putstr_fd("An unspecified error occurred.\n", 2);
	if (free_msg == 1)
		free(msg);
	bi_exit(ms, ex_code, 1);
}

/**
 * @brief 2차원 배열 복제 실패 시 호출되는 에러 핸들러입니다.
 */
void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
	(void)arr; // 사용되지 않는 변수 명시
	(void)j;
	ms_error(ms, "Array duplication failed", 1, 0);
}
