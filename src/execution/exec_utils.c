/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* exec_utils.c                                       :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:30:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 07:30:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 에러 메시지를 "minishell: name: error" 형식으로 결합하여 출력합니다.
 */
void	error_join(t_ms *ms, char *name, char *error)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(name, ": ");
	if (!temp)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	temp2 = ft_strjoin(temp, error);
	if (!temp2)
	{
		free(temp);
		ms_error(ms, "ft_strjoin failure", 1, 0);
	}
	ft_putendl_fd(temp2, 2);
	free(temp);
	free(temp2);
}

/**
 * @brief 열려 있는 모든 파이프 FD를 닫습니다.
 */
void	close_pipes(t_ms *ms)
{
	if (ms->ms_fd[0] != -1)
	{
		close(ms->ms_fd[0]);
		ms->ms_fd[0] = -1;
	}
	if (ms->ms_fd[1] != -1)
	{
		close(ms->ms_fd[1]);
		ms->ms_fd[1] = -1;
	}
	if (ms->prev_fd != -1)
	{
		close(ms->prev_fd);
		ms->prev_fd = -1;
	}
}

/**
 * @brief 열려 있는 모든 파일 디스크립터를 닫습니다.
 */
void	close_fd(t_ms *ms)
{
	if (ms->fd_in != -1)
	{
		close(ms->fd_in);
		ms->fd_in = -1;
	}
	if (ms->fd_out != -1)
	{
		close(ms->fd_out);
		ms->fd_out = -1;
	}
	close_pipes(ms);
}

/**
 * @brief 표준 입출력을 원래대로 복구합니다.
 */
void	reset_std(t_ms *ms)
{
	if (ms->reset[0] == 1)
	{
		if (dup2(ms->std_copy[0], STDIN_FILENO) < 0)
			ms_error(ms, "stdin reset failed", 1, 0);
		close(ms->std_copy[0]);
		ms->reset[0] = 0;
	}
	if (ms->reset[1] == 1)
	{
		if (dup2(ms->std_copy[1], STDOUT_FILENO) < 0)
			ms_error(ms, "stdout reset failed", 1, 0);
		close(ms->std_copy[1]);
		ms->reset[1] = 0;
	}
}
