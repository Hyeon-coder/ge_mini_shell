/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 05:43:44 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/01 03:53:54 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(t_ms *ms, t_cmd *cmd)
{
	int		i;
	bool	newline;
	int		j;

	newline = true;
	i = 1;
	while (cmd->full_cmd[i])
	{
		if (ft_strncmp(cmd->full_cmd[i], "-n", 2) == 0)
		{
			j = 1;
			while (cmd->full_cmd[i][j] == 'n')
				j++;
			if (cmd->full_cmd[i][j] == '\0')
			{
				newline = false;
				i++;
				continue;
			}
		}
		break;
	}
	while (cmd->full_cmd[i])
	{
		ft_putstr_fd(cmd->full_cmd[i], 1);
		if (cmd->full_cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	ms->exit_status = 0;
}
