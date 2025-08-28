/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:09:06 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 02:09:08 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Executes the 'env' builtin command.
** Prints the environment variables.
** Only variables with a value (containing '=') are printed.
*/
void	builtin_env(t_ms *ms, t_cmd *cmd)
{
	int	i;

	if (cmd->full_cmd[1])
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(cmd->full_cmd[1], 2);
		ft_putendl_fd("’: No such file or directory", 2);
		return ;
	}
	i = 0;
	while (ms->envp[i])
	{
		if (ft_strchr(ms->envp[i], '='))
			ft_putendl_fd(ms->envp[i], 1);
		i++;
	}
}