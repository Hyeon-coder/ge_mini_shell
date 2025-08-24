/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_cd.c                                       :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
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
			print_error("cd", "HOME not set", 1);
		return (path);
	}
	return (args[1]);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*cwd;

	path = get_target_path(args, shell);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		print_error("cd", path, 1);
		perror(NULL);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_env_value(&(shell->env_list), "PWD", cwd);
		free(cwd);
	}
	return (0);
}
