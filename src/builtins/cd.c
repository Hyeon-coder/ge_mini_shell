/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:05:44 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/02 15:23:25 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handles path expansion for paths starting with '~/'.
*/
static char	*expand_tilde_prefix(t_ms *ms, char *path)
{
	char	*home;

	home = find_var(ms, ms->envp, "HOME");
	if (!home)
		return (NULL);
	return (ft_strjoin(home, path + 1));
}

/**
 * Finds a path from an environment variable, handling errors.
 */
static char	*get_path_from_env(t_ms *ms, char *var, char *err_msg)
{
	char	*env_path;

	env_path = find_var(ms, ms->envp, var);
	if (!env_path)
	{
		ft_putendl_fd(err_msg, 2);
		return (NULL);
	}
	return (ft_strdup(env_path));
}

/**
 * Determines the target path for the 'cd' command.
 */
static char	*get_target_path(t_ms *ms, t_cmd *cmd)
{
	char	*path;

	path = cmd->full_cmd[1];
	if (!path || ft_strcmp(path, "~") == 0)
		return (get_path_from_env(ms, "HOME", "minishell: cd: HOME not set"));
	if (ft_strcmp(path, "-") == 0)
		return (get_path_from_env(ms, "OLDPWD",
				"minishell: cd: OLDPWD not set"));
	if (ft_strncmp(path, "~/", 2) == 0)
		return (expand_tilde_prefix(ms, path));
	return (ft_strdup(path));
}

/*
** Updates the PWD and OLDPWD environment variables after a successful cd.
*/
static void	update_pwd_variables(t_ms *ms)
{
	char	cwd_buffer[1024];
	char	*old_pwd;

	old_pwd = find_var(ms, ms->envp, "PWD");
	if (old_pwd)
		set_env_var(ms, "OLDPWD", old_pwd);
	if (getcwd(cwd_buffer, sizeof(cwd_buffer)))
		set_env_var(ms, "PWD", cwd_buffer);
}

/*
** Executes the 'cd' builtin command.
*/
void	builtin_cd(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	bool	is_oldpwd;

	is_oldpwd = (cmd->full_cmd[1] && ft_strcmp(cmd->full_cmd[1], "-") == 0);
	path = get_target_path(ms, cmd);
	if (!path)
	{
		ms->exit_status = 1;
		return ;
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->full_cmd[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		ms->exit_status = 1;
	}
	else
	{
		update_pwd_variables(ms);
		if (is_oldpwd)
			builtin_pwd(ms);
	}
	free(path);
}
