/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:24:47 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/18 00:31:40 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Combines and prints an error message to standard error.
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

// Closes all file descriptors related to pipes.
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

// Closes file descriptors for redirections and pipes.
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

// Restores standard input and output to their original state.
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

// Counts the number of commands in the AST.
int	count_cmds(t_ast *ast)
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
