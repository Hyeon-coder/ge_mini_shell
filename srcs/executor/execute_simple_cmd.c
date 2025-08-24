/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* execute_simple_cmd.c                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 17:00:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_ast_node *node, t_shell *shell)
{
	char	*path;
	char	**envp;

	setup_signals_child();
	if (apply_redirections(node->redirs) == -1)
		exit(1);
	if (!node->args || !node->args[0])
		exit(0);
	if (is_builtin(node->args[0]))
		exit(execute_builtin(node->args, shell));
	path = find_command_path(node->args[0], shell->env_list);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	envp = env_list_to_array(shell->env_list);
	execve(path, node->args, envp);
	print_error("execve", node->args[0], 126);
	free(path);
	free_string_array(envp);
	exit(126);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	setup_signals_parent();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 2);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0 && cmd[1] != '\0')
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_simple_command(t_ast_node *node, t_shell *shell, int is_child)
{
	pid_t	pid;
	int		original_fds[2];
	int		status;

	if (is_child)
	{
		child_process(node, shell);
		exit(1);
	}
	if (!node->args || !node->args[0])
	{
		if (node->redirs)
			return (apply_redirections_for_empty(node->redirs));
		return (0);
	}
	if (is_parent_builtin(node->args[0]))
	{
		original_fds[0] = dup(STDIN_FILENO);
		original_fds[1] = dup(STDOUT_FILENO);
		if (apply_redirections(node->redirs) == -1)
			status = 1;
		else
			status = execute_builtin(node->args, shell);
		dup2(original_fds[0], STDIN_FILENO);
		dup2(original_fds[1], STDOUT_FILENO);
		close(original_fds[0]);
		close(original_fds[1]);
		return (status);
	}
	pid = fork();
	if (pid == -1)
		return (print_error("fork", NULL, 1), 1);
	if (pid == 0)
		child_process(node, shell);
	return (wait_for_child(pid));
}
