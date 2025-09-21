/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:25:02 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/22 02:36:21 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Executes a command that only consists of redirections.
void	run_redir(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1] == 0)
	{
		reset_child_signals();
		if (handle_files(ms, ast->cmd))
			bi_exit(ms, 1, 1);
		// reset_std(ms);
		// close_fd(ms);
		bi_exit(ms, 0, 1);
	}
}

// Handles pipelines that contain only redirections without a command.
// void	run_no_cmd(t_ms *ms, t_ast *ast)
// {
// 	if (pipe(ms->ms_fd) < 0)
// 		ms_error(ms, "Pipe failure", 1, 0);
// 	run_redir(ms, ast);
// 	if (ms->cmd_no > 1)
// 		next_pipe(ms, ms->cmd_index + 1 >= ms->cmd_no);
// 	reset_std(ms);
// }
