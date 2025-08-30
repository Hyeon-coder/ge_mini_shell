/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:17:39 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/30 22:12:52 by JuHyeon          ###   ########.fr       */
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
		else if (sig == SIGQUIT)
			ft_putendl_fd("^\\Quit (core dumped)", STDERR_FILENO);
	}
	while (wait(NULL) > 0)
		;
}

/*
** Creates and connects the child processes for the intermediate
** commands in the pipeline.
*/
static void	create_pipe_processes(t_ms *ms, t_ast **ast, int *in_fd)
{
	pid_t	pid;
	int		pipe_fds[2];

	while ((*ast)->type == NODE_PIPE)
	{
		if (pipe(pipe_fds) == -1)
			ms_error(ms, "pipe failed", 1, 0);
		pid = fork();
		if (pid == -1)
			ms_error(ms, "fork failed", 1, 0);
		if (pid == 0)
		{
			close(pipe_fds[0]);
			child_process_routine(ms, (*ast)->left, *in_fd, pipe_fds[1]);
		}
		close(pipe_fds[1]);
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		*in_fd = pipe_fds[0];
		*ast = (*ast)->right;
	}
}

/*
** Executes a command pipeline. It creates intermediate processes for
** piped commands and a final process for the last command in the pipeline.
*/
void	execute_pipeline(t_ms *ms, t_ast *ast)
{
	pid_t	pid;
	int		in_fd;

	in_fd = STDIN_FILENO;
	set_noninteractive_signals();
	create_pipe_processes(ms, &ast, &in_fd);
	pid = fork();
	if (pid == -1)
		ms_error(ms, "fork failed", 1, 0);
	if (pid == 0)
		child_process_routine(ms, ast, in_fd, STDOUT_FILENO);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	wait_for_children(ms, pid);
	set_interactive_signals();
}
