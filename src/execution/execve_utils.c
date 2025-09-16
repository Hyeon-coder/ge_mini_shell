/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:24:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/16 13:35:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(t_ms *ms, char *cmd)
{
	char	**paths;
	char	*path_part;
	char	*path;
	int		i;

	path = find_var(ms, ms->envp, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_free_array(paths);
			return (path);
		}
		free(path);
	}
	ft_free_array(paths);
	return (NULL);
}

void	run_execve(t_ms *ms, t_cmd *cmd)
{
	char	*path;

	path = NULL;
	if (cmd->full_cmd && cmd->full_cmd[0])
	{
		if (ft_strchr(cmd->full_cmd[0], '/'))
			path = ft_strdup(cmd->full_cmd[0]);
		else
			path = get_path(ms, cmd->full_cmd[0]);
	}
	if (!path || access(path, F_OK) != 0)
	{
		error_join(ms, cmd->full_cmd[0], "command not found");
		bi_exit(ms, 127, 1);
	}
	if (access(path, X_OK) != 0)
	{
		error_join(ms, cmd->full_cmd[0], "Permission denied");
		bi_exit(ms, 126, 1);
	}
	if (ms->std_copy[0] != -1)
		close(ms->std_copy[0]);
	if (ms->std_copy[1] != -1)
		close(ms->std_copy[1]);
	close_pipes(ms);
	execve(path, cmd->full_cmd, ms->envp);
	error_join(ms, cmd->full_cmd[0], strerror(errno));
	free(path);
	bi_exit(ms, 127, 1);
}
