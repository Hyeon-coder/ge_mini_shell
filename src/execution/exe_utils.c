/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:51:21 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/10 01:29:56 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Closes any open file descriptors and pipes
** Thorough cleanup of all file descriptors
*/
void	close_fd(t_ms *ms)
{
	if (!ms)
		return ;
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
	if (ms->std_copy[0] != -1)
	{
		close(ms->std_copy[0]);
		ms->std_copy[0] = -1;
	}
	if (ms->std_copy[1] != -1)
	{
		close(ms->std_copy[1]);
		ms->std_copy[1] = -1;
	}
}

/*
** Closes any open pipes and resets file descriptors
*/
void	close_pipes(t_ms *ms)
{
	if (!ms)
		return ;
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
** Complete cleanup of environment variables
** Thorough deallocation of all environment memory
*/
void	free_envp(t_ms *ms)
{
	int	i;

	if (!ms || !ms->envp)
		return ;
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
	free(ms->envp);
	ms->envp = NULL;
	ms->elements = 0;
}

/*
** Force cleanup all parsing structures to prevent memory leaks
*/
static void	force_cleanup_parsing(t_ms *ms)
{
	if (ms->ast)
	{
		free_ast(ms->ast);
		ms->ast = NULL;
		ms->cmd = NULL;
	}
	if (ms->token)
	{
		free_tokens(ms->token);
		ms->token = NULL;
	}
	if (ms->word)
	{
		free_word(ms->word);
		ms->word = NULL;
	}
	if (ms->cmd)
	{
		free_cmd(ms->cmd);
		ms->cmd = NULL;
	}
}

/*
** Complete child process cleanup
** All memory and resources freed before exit
*/
void	complete_child_cleanup(t_ms *ms)
{
	if (!ms)
		return ;
	close_fd(ms);
	free_execution_memory(ms);
	force_cleanup_parsing(ms);
	free_envp(ms);
	if (ms->prompt)
	{
		free(ms->prompt);
		ms->prompt = NULL;
	}
	clean_all_heredocs(ms);
	rl_clear_history();
}
