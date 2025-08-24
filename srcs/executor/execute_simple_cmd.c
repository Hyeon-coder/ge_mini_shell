/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* execute_simple_cmd.c                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 13:30:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_ast_node *node, t_shell *shell)
{
	char	*path;
	char	**envp;

	if (apply_redirections(node->redirs) == -1)
		exit(1);
	if (!node->args || !node->args[0])
		exit(0);
	if (is_builtin(node->args[0]))
		exit(execute_builtin(node->args, shell));
	path = find_command_path(node->args[0], shell->env_list);
	if (!path)
	{
		print_error(node->args[0], "command not found", 127);
		exit(127);
	}
	envp = env_list_to_array(shell->env_list);
	execve(path, node->args, envp);
	print_error("execve failed", node->args[0], 126);
	free(path);
	free_string_array(envp);
	exit(126);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_simple_command(t_ast_node *node, t_shell *shell)
{
	pid_t	pid;
	int		original_fds[2];
	int		status;

	if (node->redirs)
	{
		original_fds[0] = dup(STDIN_FILENO);
		original_fds[1] = dup(STDOUT_FILENO);
		if (apply_redirections(node->redirs) == -1)
			return (1);
	}
	if (!node->args || !node->args[0])
	{
		if (node->redirs)
		{
			dup2(original_fds[0], STDIN_FILENO);
			dup2(original_fds[1], STDOUT_FILENO);
			close(original_fds[0]);
			close(original_fds[1]);
		}
		return (0);
	}
	if (is_parent_builtin(node->args[0]))
	{
		status = execute_builtin(node->args, shell);
		if (node->redirs)
		{
			dup2(original_fds[0], STDIN_FILENO);
			dup2(original_fds[1], STDOUT_FILENO);
			close(original_fds[0]);
			close(original_fds[1]);
		}
		return (status);
	}
	pid = fork();
	if (pid == -1)
		return (print_error("fork failed", NULL, 1), 1);
	if (pid == 0)
		child_process(node, shell);
	if (node->redirs)
	{
		dup2(original_fds[0], STDIN_FILENO);
		dup2(original_fds[1], STDOUT_FILENO);
		close(original_fds[0]);
		close(original_fds[1]);
	}
	return (wait_for_child(pid));
}
