/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:36:41 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/30 00:50:13 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_ms *ms, t_cmd *cmd)
{
	if (ft_strcmp(cmd->full_cmd[0], "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmd->full_cmd[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->full_cmd[0], "cd") == 0)
		builtin_cd(ms, cmd);
	else if (ft_strcmp(cmd->full_cmd[0], "exit") == 0)
		builtin_exit(ms, cmd);
	else if (ft_strcmp(cmd->full_cmd[0], "env") == 0)
		builtin_env(ms, cmd);
	else if (ft_strcmp(cmd->full_cmd[0], "export") == 0)
		builtin_export(ms, cmd);
	else if (ft_strcmp(cmd->full_cmd[0], "unset") == 0)
		builtin_unset(ms, cmd);
}

void	execute_child_process(t_ms *ms, t_cmd *cmd, char *path)
{
	reset_child_signals();
	execve(path, cmd->full_cmd, ms->envp);
    free(path);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->full_cmd[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	exit(126);
}

void	wait_for_child_process(t_ms *ms, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->exit_status = 128 + WTERMSIG(status);
}
