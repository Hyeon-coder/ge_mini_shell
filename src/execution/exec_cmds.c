/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:22:34 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/22 02:28:00 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Check if a command is a built-in command.
int	check_for_bi(t_cmd *cmd)
{
	if (!cmd || !cmd->full_cmd || !cmd->full_cmd[0])
		return (0);
	if (!ft_strcmp(cmd->full_cmd[0], "cd")
		|| !ft_strcmp(cmd->full_cmd[0], "env")
		|| !ft_strcmp(cmd->full_cmd[0], "export")
		|| !ft_strcmp(cmd->full_cmd[0], "unset")
		|| !ft_strcmp(cmd->full_cmd[0], "echo")
		|| !ft_strcmp(cmd->full_cmd[0], "pwd")
		|| !ft_strcmp(cmd->full_cmd[0], "exit"))
		return (1);
	return (0);
}

// Executes a command by calling either a built-in function or execve
void	run_cmd(t_ms *ms, t_cmd *cmd)
{
	if (check_for_bi(cmd))
	{
		if (!ft_strcmp(cmd->full_cmd[0], "echo"))
			builtin_echo(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "cd"))
			builtin_cd(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "pwd"))
			builtin_pwd(ms);
		else if (!ft_strcmp(cmd->full_cmd[0], "export"))
			builtin_export(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "unset"))
			builtin_unset(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "env"))
			builtin_env(ms, cmd);
		else if (!ft_strcmp(cmd->full_cmd[0], "exit"))
			builtin_exit(ms, cmd);
	}
	else
		run_execve(ms, cmd);
	if (ms->child == true)
		bi_exit(ms, ms->exit_status, 1);
}

// Executes a single command without pipes.
void	run_one(t_ms *ms, t_cmd *cmd)
{
	if (check_for_bi(cmd))
	{
		ms->child = false;
		if (handle_files(ms, cmd))
			return ;
		if (g_signal == SIGINT)
			return ;
		run_cmd(ms, cmd);
		return ;
	}
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0)
	{
		reset_child_signals();
		if (handle_files(ms, cmd) || g_signal)
			bi_exit(ms, 1, 1);
		run_cmd(ms, cmd);
	}
}

// Dispatches commands for execution based on their position in a pipeline.
void	start_cmds(t_ms *ms, t_ast *ast)
{
	if (ms->cmd_no == 1)
	{
		if (ast->cmd && ast->cmd->full_cmd)
			run_one(ms, ast->cmd);
		else
			run_redir(ms, ast); // 단일 리디렉션만 있는 경우
	}
	else
	{
		if (ms->cmd_index == 1)
			exec_first_pipe(ms, ast);
		else if (ms->cmd_index == ms->cmd_no)
			exec_last_pipe(ms, ast);
		else
			exec_mid_pipe(ms, ast);
	}
}

// Recursively traverses the AST to parse and dispatch commands for execution.
void	parse_cmds(t_ms *ms, t_ast *ast)
{
	if (!ast || g_signal == SIGINT)
		return ;
	if (ast->type == NODE_PIPE)
	{
		parse_cmds(ms, ast->left);
		next_pipe(ms, (ms->cmd_index + 1) >= ms->cmd_no);
		parse_cmds(ms, ast->right);
	}
	else if (ast->type == NODE_COMMAND || ast->type == NODE_MISSCMD)
	{
		ms->cmd_index++;
		// if (ast->cmd && ast->cmd->full_cmd)
			start_cmds(ms, ast);
		// else
		// 	run_no_cmd(ms, ast);
	}
}
