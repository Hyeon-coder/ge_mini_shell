/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_exit.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 13:30:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	if (isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", 2);
	if (!args[1])
		exit(shell->last_exit_status);
	if (!is_numeric(args[1]))
	{
		print_error("exit", "numeric argument required", 255);
		exit(255);
	}
	if (args[2])
	{
		print_error("exit", "too many arguments", 1);
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	exit((unsigned char)exit_code);
}
