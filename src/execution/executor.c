/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 00:20:00 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 06:40:06 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(char *cmd);
static void	execute_builtin(t_ms *ms, t_cmd *cmd);
static void	run_external_cmd(t_ms *ms, t_cmd *cmd, char *path);
static bool	setup_redirections(t_cmd *cmd, int *og_stdin, int *og_stdout);
static void	dispatch_command(t_ms *ms, t_cmd *cmd);

static int	is_builtin(char *cmd)
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

static void	execute_builtin(t_ms *ms, t_cmd *cmd)
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

static void	run_external_cmd(t_ms *ms, t_cmd *cmd, char *path)
{
	pid_t	pid;
	int		status;

	set_noninteractive_signals();
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return ;
	}
	if (pid == 0)
	{
		reset_child_signals();
		execve(path, cmd->full_cmd, ms->envp);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->full_cmd[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit(127);
	}
	waitpid(pid, &status, 0);
	set_interactive_signals();
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->exit_status = 128 + WTERMSIG(status);
	free(path);
}

static bool	setup_redirections(t_cmd *cmd, int *og_stdin, int *og_stdout)
{
	*og_stdin = handle_input_redirection(cmd);
	if (*og_stdin == -1)
		return (false);
	*og_stdout = handle_output_redirection(cmd);
	if (*og_stdout == -1)
	{
		restore_input(*og_stdin);
		return (false);
	}
	return (true);
}

static void	dispatch_command(t_ms *ms, t_cmd *cmd)
{
	char	*cmd_path;

	if (is_builtin(cmd->full_cmd[0]))
		execute_builtin(ms, cmd);
	else
	{
		cmd_path = get_command_path(ms, cmd->full_cmd[0]);
		if (!cmd_path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->full_cmd[0], 2);
			ft_putendl_fd(": command not found", 2);
			ms->exit_status = 127;
		}
		else
			run_external_cmd(ms, cmd, cmd_path);
	}
}

void	execute_simple_command(t_ms *ms, t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;

	if (!setup_redirections(cmd, &original_stdin, &original_stdout))
	{
		ms->exit_status = 1;
		return ;
	}
	if (!cmd->full_cmd || !cmd->full_cmd[0])
	{
		restore_input(original_stdin);
		restore_output(original_stdout);
		return ;
	}
	dispatch_command(ms, cmd);
	restore_input(original_stdin);
	restore_output(original_stdout);
}

void	run_executor(t_ms *ms, t_ast *ast)
{
	if (!ast || ms->heredoc_stop)
		return ;
	if (ast->type == NODE_PIPE)
		execute_pipeline(ms, ast);
	else if (ast->type == NODE_COMMAND)
	{
		if (ast->cmd)
			execute_simple_command(ms, ast->cmd);
	}
}
