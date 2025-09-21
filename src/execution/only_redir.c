/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:25:02 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/22 02:49:26 by JuHyeon          ###   ########.fr       */
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
		bi_exit(ms, 0, 1);
	}
}
