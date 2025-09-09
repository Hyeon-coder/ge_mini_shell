/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:51:21 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/09 23:02:52 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

// exe_utils.c 파일 끝에 추가할 함수들

/*
** Closes any open file descriptors
** Calls close_pipes()
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

/*
** Closes any open pipes if open
** Resets any closed pipes to -1
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

/*
** Frees the memory in the envp
** Frees the envp pointer itself
** Sets envp pointer to null
*/
void	free_envp(t_ms *ms)
{
	int	i;

	i = -1;
	if (ms->envp)
	{
		while (ms->envp[++i])
		{
			free(ms->envp[i]);
			ms->envp[i] = NULL; // 추가: NULL로 설정
		}
		free(ms->envp);
		ms->envp = NULL;
	}
}
