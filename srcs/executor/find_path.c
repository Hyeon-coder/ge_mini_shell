/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* find_path.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*path_part;
	char	*full_path;

	i = 0;
	while (paths && paths[i])
	{
		path_part = ft_strjoin(paths[i], "/");
		if (!path_part)
			return (NULL);
		full_path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = search_in_paths(cmd, paths);
	free_string_array(paths);
	return (full_path);
}
