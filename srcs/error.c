/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* error.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 19:30:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	set_exit_status(int status)
{
	g_exit_status = status;
	return (status);
}

void	print_error(char *s1, char *s2, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(s2, 2);
	ft_putchar_fd('\n', 2);
	g_exit_status = exit_code;
}