/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:32:27 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 00:32:29 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Joins the directory path and command name to form a full path.
** Example: ("/bin", "ls") -> "/bin/ls"
*/
static char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

/*
** Searches through the directories specified in the PATH variable
** to find an executable command.
*/
static char	*search_in_paths(char **dirs, char *cmd)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (dirs[i])
	{
		cmd_path = join_path(dirs[i], cmd);
		if (!cmd_path)
		{
			ft_free_array(dirs);
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			ft_free_array(dirs);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}

/*
** Finds the full path of a command.
** 1. If cmd is an absolute/relative path, it checks its validity.
** 2. Otherwise, it searches for the command in the PATH directories.
*/
char	*get_command_path(t_ms *ms, char *cmd)
{
	char	*path_var;
	char	**dirs;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_var = find_var(ms, ms->envp, "PATH");
	if (!path_var)
		return (NULL);
	dirs = ft_split(path_var, ':');
	if (!dirs)
		return (NULL);
	return (search_in_paths(dirs, cmd));
}
