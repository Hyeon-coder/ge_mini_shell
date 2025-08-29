/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* pipe.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 05:00:00 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 05:30:00 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_routine(t_ms *ms, t_ast *node, int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (node->type == NODE_PIPE)
		run_executor(ms, node);
	else if (node->cmd && node->cmd->full_cmd)
		execute_simple_command(ms, node->cmd);
	exit(ms->exit_status);
}

static void	wait_for_children(t_ms *ms, pid_t last_pid)
{
	int	status;
	int	exit_code;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		ms->exit_status = exit_code;
	}
	else if (WIFSIGNALED(status))
	{
		exit_code = 128 + WTERMSIG(status);
		ms->exit_status = exit_code;
	}
	while (wait(NULL) > 0)
		;
}

void	execute_pipeline(t_ms *ms, t_ast *ast)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		in_fd;

	in_fd = STDIN_FILENO;
	while (ast->type == NODE_PIPE)
	{
		pipe(pipe_fds);
		pid = fork();
		if (pid == 0)
			child_process_routine(ms, ast->left, in_fd, pipe_fds[1]);
		close(pipe_fds[1]);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		in_fd = pipe_fds[0];
		ast = ast->right;
	}
	pid = fork();
	if (pid == 0)
		child_process_routine(ms, ast, in_fd, STDOUT_FILENO);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	wait_for_children(ms, pid);
}
