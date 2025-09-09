/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:51:21 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/09 21:55:02 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

// minishell_team/src/execution/exec_master.c 파일의 일부
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

// minishell_team/src/execution/exec_utils.c 파일의 일부
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
			free(ms->envp[i]);
		free(ms->envp);
		ms->envp = NULL;
	}
}
