/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:17:52 by mpierce           #+#    #+#             */
/*   Updated: 2025/09/10 01:27:40 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Free single infile structure completely
** Ensures all nested allocations are freed
*/
static void	free_single_infile(t_infile *infile)
{
	if (!infile)
		return ;
	if (infile->name)
	{
		free(infile->name);
		infile->name = NULL;
	}
	free(infile);
}

/*
** Completely free infile structures including all nested allocations
** Handles NULL arrays safely
*/
void	ft_free_infile(t_infile **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free_single_infile(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

/*
** Free all cmd arrays and reset pointers to prevent double free
*/
static void	free_cmd_arrays(t_cmd *cmd)
{
	if (cmd->full_cmd)
	{
		ft_free_array(cmd->full_cmd);
		cmd->full_cmd = NULL;
	}
	if (cmd->infile)
	{
		ft_free_infile(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		ft_free_array(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->append)
	{
		free(cmd->append);
		cmd->append = NULL;
	}
}

/*
** Completely free cmd structure with thorough cleanup
*/
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_cmd_arrays(cmd);
	free(cmd);
}

/*
** Complete cleanup for child process - frees execution-related memory
*/
void	free_execution_memory(t_ms *ms)
{
	if (!ms)
		return ;
	if (ms->cmd_path)
	{
		free(ms->cmd_path);
		ms->cmd_path = NULL;
	}
	if (ms->heredoc_name)
	{
		free(ms->heredoc_name);
		ms->heredoc_name = NULL;
	}
	if (ms->pids)
	{
		free(ms->pids);
		ms->pids = NULL;
	}
}
