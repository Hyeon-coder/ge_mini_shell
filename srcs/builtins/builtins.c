/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:54:14 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 10:54:14 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(cmd, "cd") == 0)
		return (builtin_cd(args, shell));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd, "export") == 0)
		return (builtin_export(args, shell));
	if (ft_strcmp(cmd, "unset") == 0)
		return (builtin_unset(args, shell));
	if (ft_strcmp(cmd, "env") == 0)
		return (builtin_env(shell));
	if (ft_strcmp(cmd, "exit") == 0)
		return (builtin_exit(args, shell));
	return (127);
}
