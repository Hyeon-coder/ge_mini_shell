/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* execute_pipeline.c                                 :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 17:00:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipe_child(t_ast_node *node, int *pipe_fd,
	int direction, t_shell *shell)
{
	if (direction == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	exit(_executor(node, shell, 1));
}

int	execute_pipeline(t_ast_node *node, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (pipe(pipe_fd) == -1)
		return (print_error("pipe", NULL, 1), 1);
	pid_left = fork();
	if (pid_left == 0)
		execute_pipe_child(node->left, pipe_fd, 0, shell);
	pid_right = fork();
	if (pid_right == 0)
		execute_pipe_child(node->right, pipe_fd, 1, shell);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	setup_signals_parent();
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	setup_signals();
	if (WIFEXITED(status_right))
		return (WEXITSTATUS(status_right));
	if (WIFSIGNALED(status_right))
	{
		if (WTERMSIG(status_right) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 2);
		return (128 + WTERMSIG(status_right));
	}
	return (1);
}
