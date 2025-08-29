/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:36:32 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 17:36:33 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_external_cmd(t_ms *ms, t_cmd *cmd, char *path);
static bool	setup_redirections(t_cmd *cmd, int *og_stdin, int *og_stdout);
static void	dispatch_command(t_ms *ms, t_cmd *cmd);

static void	run_external_cmd(t_ms *ms, t_cmd *cmd, char *path)
{
	pid_t	pid;

	set_noninteractive_signals();
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return ;
	}
	if (pid == 0)
		execute_child_process(ms, cmd, path);
	wait_for_child_process(ms, pid);
	set_interactive_signals();
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
