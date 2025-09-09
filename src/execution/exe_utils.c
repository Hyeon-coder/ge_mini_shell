/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:51:21 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/10 00:16:20 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Closes any open file descriptors and pipes
*/
void	close_fd(t_ms *ms)
{
	if (!ms)
		return ;
	
	// ⭐ 입력 파일 디스크립터 정리
	if (ms->fd_in != -1)
	{
		close(ms->fd_in);
		ms->fd_in = -1;
	}
	
	// ⭐ 출력 파일 디스크립터 정리
	if (ms->fd_out != -1)
	{
		close(ms->fd_out);
		ms->fd_out = -1;
	}
	
	// ⭐ 모든 파이프 정리
	close_pipes(ms);
}

/*
** Closes any open pipes and resets file descriptors
*/
void	close_pipes(t_ms *ms)
{
	if (!ms)
		return ;
	
	// ⭐ 현재 파이프 읽기 끝 정리
	if (ms->ms_fd[0] != -1)
	{
		close(ms->ms_fd[0]);
		ms->ms_fd[0] = -1;
	}
	
	// ⭐ 현재 파이프 쓰기 끝 정리
	if (ms->ms_fd[1] != -1)
	{
		close(ms->ms_fd[1]);
		ms->ms_fd[1] = -1;
	}
	
	// ⭐ 이전 파이프 디스크립터 정리
	if (ms->prev_fd != -1)
	{
		close(ms->prev_fd);
		ms->prev_fd = -1;
	}
}

/*
** Complete cleanup of environment variables
** main에서 복사된 환경변수들을 execution에서 완전 해제
*/
void	free_envp(t_ms *ms)
{
	int	i;

	if (!ms || !ms->envp)
		return ;
	
	// ⭐ 각 환경변수 문자열 정리 (main에서 ft_strdup으로 생성됨)
	i = 0;
	while (ms->envp[i])
	{
		if (ms->envp[i])
		{
			free(ms->envp[i]);
			ms->envp[i] = NULL;
		}
		i++;
	}
	
	// ⭐ 환경변수 배열 자체 정리 (main에서 malloc으로 생성됨)
	free(ms->envp);
	ms->envp = NULL;
	
	// ⭐ 요소 개수 초기화
	ms->elements = 0;
}
