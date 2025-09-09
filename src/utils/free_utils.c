/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:17:52 by mpierce           #+#    #+#             */
/*   Updated: 2025/09/10 00:19:08 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Free single infile structure completely
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
** Free cmd arrays and reset pointers
*/
void	free_cmd_help(t_cmd *cmd)
{
	if (!cmd)
		return ;
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
** Completely free cmd structure
*/
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_cmd_help(cmd);
	free(cmd);
}
