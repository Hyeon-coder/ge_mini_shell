/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:14 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/03 14:45:19 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Executes the 'pwd' builtin command.
** Prints the current working directory.
*/
void builtin_pwd(t_ms *ms)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		cwd = find_var(ms, ms->envp, "PWD");
		if (cwd)
			ft_putendl_fd(cwd, 1);
		else
		{
			ft_putstr_fd("minishell: pwd: ", 2);
			ft_putendl_fd(strerror(errno), 2);
			ms->exit_status = 1;
		}
		return;
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	ms->exit_status = 0;
}
