/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:29:03 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h> // perror

static void	child_process(t_ast_node *node, t_shell *shell)
{
	char	*path;
	char	**envp;

    setup_signals_child();

    // [중요] 리디렉션은 자식 프로세스에서 적용 (테스트 16, 17, 18)
	if (apply_redirections(node->redirs) == -1)
		exit(1); // 리디렉션 실패 시 종료 (테스트 21)

	if (!node->args || !node->args[0])
		exit(0);
	if (is_builtin(node->args[0]))
		exit(execute_builtin(node->args, shell));

	path = find_command_path(node->args[0], shell->env_list);
	if (!path)
	{
        // 에러 메시지 표준화 (테스트 27)
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(node->args[0], STDERR_FILENO);
        if (ft_strchr(node->args[0], '/'))
             ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        else
		    ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	envp = env_list_to_array(shell->env_list);
	execve(path, node->args, envp);

    // execve 실패 시
    perror("minishell: execve failed");
	exit(126);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

    setup_signals_parent(); // 대기 중 시그널 무시
	waitpid(pid, &status, 0);
    setup_signals(); // 시그널 복구

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
            ft_putchar_fd('\n', STDOUT_FILENO);
        else if (WTERMSIG(status) == SIGQUIT)
            ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
		return (128 + WTERMSIG(status));
    }
	return (1);
}

static int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

// 부모 내장 명령어 실행 시 FD 백업 및 복원
static int execute_parent_builtin(t_ast_node *node, t_shell *shell)
{
    int original_fds[2];
    int status;

    if (!node->redirs)
        return (execute_builtin(node->args, shell));

    original_fds[0] = dup(STDIN_FILENO);
    original_fds[1] = dup(STDOUT_FILENO);

    if (apply_redirections(node->redirs) == -1)
    {
        if (original_fds[0] != -1) close(original_fds[0]);
        if (original_fds[1] != -1) close(original_fds[1]);
        return (1);
    }

    status = execute_builtin(node->args, shell);

    dup2(original_fds[0], STDIN_FILENO);
    dup2(original_fds[1], STDOUT_FILENO);
    close(original_fds[0]);
    close(original_fds[1]);
    return (status);
}

// 함수 시그니처 변경 (헤더 파일 수정 필요)
int	execute_simple_command(t_ast_node *node, t_shell *shell, int is_child)
{
	pid_t	pid;

    // 1. 파이프라인 내부 실행 (이미 포크된 상태)
    if (is_child)
    {
        child_process(node, shell);
        // child_process는 항상 exit() 하므로 반환되지 않음
    }

    // --- 여기서부터는 is_child == 0 (부모 컨텍스트) ---

    // 2. 명령어가 없는 경우 (테스트 19)
	if (!node->args || !node->args[0])
	{
		if (node->redirs)
			return (apply_redirections_for_empty(node->redirs));
		return (0);
	}

    // 3. 부모에서 실행해야 하는 내장 명령어
	if (is_parent_builtin(node->args[0]))
	{
        // export는 인자가 없을 경우 자식에서 실행되어야 출력이 리디렉션/파이프 될 수 있음
        if (!(ft_strcmp(node->args[0], "export") == 0 && node->args[1] == NULL))
		    return (execute_parent_builtin(node, shell));
	}

    // 4. 외부 명령어 또는 자식 내장 명령어 (포크 필요)
	pid = fork();
	if (pid == -1)
    {
        perror("minishell: fork failed");
		return (1);
    }
	if (pid == 0)
		child_process(node, shell);

	return (wait_for_child(pid));
}
