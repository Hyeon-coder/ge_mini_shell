/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:36:32 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/01 05:03:00 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dispatch_command(t_ms *ms, t_cmd *cmd);

static void	run_external_cmd(t_ms *ms, t_cmd *cmd)
{
	pid_t	pid;
	char	*path;

	set_noninteractive_signals();
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ms->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		path = get_command_path(ms, cmd->full_cmd[0]);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->full_cmd[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
		execute_child_process(ms, cmd, path);
	}
	wait_for_child_process(ms, pid);
	set_interactive_signals();
}

bool	setup_redirections(t_cmd *cmd, int *og_stdin, int *og_stdout)
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
	if (is_builtin(cmd->full_cmd[0]))
		execute_builtin(ms, cmd);
	else
		run_external_cmd(ms, cmd);
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
    // 실행할 명령어가 없는 경우, 리다이렉션이 성공했으므로 종료 코드는 0
    if (!cmd->full_cmd || !cmd->full_cmd[0])
    {
        ms->exit_status = 0; // bash와 동일하게 0으로 설정
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
    else if (ast->type == NODE_COMMAND || ast->type == NODE_MISSCMD)
    {
        if (ast->cmd)
            execute_simple_command(ms, ast->cmd);
        // 아래 라인 삭제: if (ast->type == NODE_MISSCMD && ms->exit_status == 0) ms->exit_status = 1;
    }
}
