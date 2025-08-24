/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:29:17 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h> // perror

// 내부 실행기 프로토타입 (executor.c에서 정의됨)
int	_executor(t_ast_node *node, t_shell *shell, int is_child);

static void	execute_pipe_child(t_ast_node *node, int *pipe_fd,
	int direction, t_shell *shell)
{
	if (direction == 0) // 왼쪽 자식 (쓰기)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2 failed");
            exit(1);
        }
		close(pipe_fd[1]);
	}
	else // 오른쪽 자식 (읽기)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        {
            perror("minishell: dup2 failed");
            exit(1);
        }
		close(pipe_fd[0]);
	}
    // 내부 실행기 호출 (is_child = 1)
	exit(_executor(node, shell, 1));
}

static int get_pipeline_status(int status_right)
{
    int final_status;

	if (WIFEXITED(status_right))
		final_status = WEXITSTATUS(status_right);
    else if (WIFSIGNALED(status_right))
    {
        final_status = 128 + WTERMSIG(status_right);
        if (WTERMSIG(status_right) == SIGINT)
            ft_putchar_fd('\n', STDOUT_FILENO);
        else if (WTERMSIG(status_right) == SIGQUIT)
             ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
    }
    else
	    final_status = 1;
    return (final_status);
}

int	execute_pipeline(t_ast_node *node, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (pipe(pipe_fd) == -1)
    {
        perror("minishell: pipe failed");
		return (1);
    }

    setup_signals_parent(); // 자식 실행 중 시그널 무시

	pid_left = fork();
    if (pid_left == -1)
    {
        perror("minishell: fork failed");
        close(pipe_fd[0]); close(pipe_fd[1]);
        setup_signals();
        return (1);
    }
	if (pid_left == 0)
		execute_pipe_child(node->left, pipe_fd, 0, shell);

	pid_right = fork();
    if (pid_right == -1)
    {
        perror("minishell: fork failed");
        close(pipe_fd[0]); close(pipe_fd[1]);
        waitpid(pid_left, &status_left, 0);
        setup_signals();
        return (1);
    }
	if (pid_right == 0)
		execute_pipe_child(node->right, pipe_fd, 1, shell);

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);

    setup_signals(); // 시그널 복구

	return (get_pipeline_status(status_right));
}
