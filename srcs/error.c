/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* error.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 13:15:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	set_exit_status(int status)
{
	g_exit_status = status;
	return (status);
}

void	print_error(char *cmd, char *arg, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		if (errno != 0)
			ft_putstr_fd(": ", 2);
	}
	if (errno != 0)
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putchar_fd('\n', 2);
	errno = 0;
	set_exit_status(exit_code);
}
