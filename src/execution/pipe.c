/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* pipe.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/* +#+           */
/* Created: 2025/08/29 05:00:00 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 05:00:02 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipe_child(t_ms *ms, t_ast *ast_node, int p_fds[2], int side)
{
	if (side == 0) // Left child (writes to pipe)
	{
		close(p_fds[0]);
		dup2(p_fds[1], STDOUT_FILENO);
		close(p_fds[1]);
	}
	else // Right child (reads from pipe)
	{
		close(p_fds[1]);
		dup2(p_fds[0], STDIN_FILENO);
		close(p_fds[0]);
	}
	run_executor(ms, ast_node);
	exit(ms->exit_status);
}

void	execute_pipe(t_ms *ms, t_ast *ast)
{
	int		pipe_fds[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pipe_fds) == -1)
		return ;
	pid_left = fork();
	if (pid_left == 0)
		execute_pipe_child(ms, ast->left, pipe_fds, 0);
	pid_right = fork();
	if (pid_right == 0)
		execute_pipe_child(ms, ast->right, pipe_fds, 1);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
}