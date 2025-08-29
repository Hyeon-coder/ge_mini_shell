/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* redirection_utils.c                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 14:52:11 by JuHyeon           #+#    #+#             */
/* Updated: 2025/08/29 14:52:11 by JuHyeon          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	process_infile_loop(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	fd = -1;
	while (cmd->infile[i])
	{
		if (fd != -1)
			close(fd);
		fd = open(cmd->infile[i]->name, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->infile[i]->name, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			return (-1);
		}
		i++;
	}
	return (fd);
}
