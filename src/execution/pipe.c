/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:17:39 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 14:17:52 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_routine(t_ms *ms, t_ast *node, int in_fd, int out_fd)
{
	reset_child_signals();
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
	int	sig;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		ms->exit_status = exit_code;
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		exit_code = 128 + sig;
		ms->exit_status = exit_code;
		if (sig == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
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
	set_noninteractive_signals();
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
	set_interactive_signals();
}
