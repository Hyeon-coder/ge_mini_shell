/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_cd.c                                       :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 17:30:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(shell->env_list, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (path);
	}
	return (args[1]);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	new_pwd[1024];

	old_pwd = get_env_value(shell->env_list, "PWD");
	path = get_target_path(args, shell);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	if (old_pwd)
		set_env_value(&(shell->env_list), "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
		set_env_value(&(shell->env_list), "PWD", new_pwd);
	else
	{
		perror("minishell: cd: getcwd failed");
		return (1);
	}
	return (0);
}
