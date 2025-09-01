/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 05:43:44 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/01 22:45:23 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Checks if an argument is a valid "-n" option (e.g., -n, -nn, -nnn).
 */
static bool	is_n_option(const char *arg)
{
	int	i;

	if (ft_strncmp(arg, "-n", 2) != 0)
		return (false);
	i = 2;
	while (arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (true);
	return (false);
}

/*
 * Parses "-n" options to determine if a newline should be printed.
 */
static int	parse_options(char **args, bool *newline_flag)
{
	int	i;

	*newline_flag = true;
	i = 1;
	while (args[i] && is_n_option(args[i]))
	{
		*newline_flag = false;
		i++;
	}
	return (i);
}

/**
 * The builtin echo command.
 * Handles the "-n" option to suppress the trailing newline.
 */
void	builtin_echo(t_ms *ms, t_cmd *cmd)
{
	int		i;
	bool	newline;

	i = parse_options(cmd->full_cmd, &newline);
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
