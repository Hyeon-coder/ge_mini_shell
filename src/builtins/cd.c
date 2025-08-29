/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:05:44 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/30 01:23:20 by JuHyeon          ###   ########.fr       */
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

/*
** Determines the target path for the 'cd' command based on its arguments.
** Handles cases for 'cd', 'cd ~', 'cd -', and 'cd ~/path'.
*/
static char	*get_target_path(t_ms *ms, t_cmd *cmd)
{
	char	*path;

	path = cmd->full_cmd[1];
	if (!path || ft_strcmp(path, "~") == 0)
	{
		path = find_var(ms, ms->envp, "HOME");
		if (!path)
			ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (ft_strdup(path));
	}
	if (ft_strcmp(path, "-") == 0)
	{
		path = find_var(ms, ms->envp, "OLDPWD");
		if (!path)
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (ft_strdup(path));
	}
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

	path = get_target_path(ms, cmd);
	if (!path)
		return ;
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->full_cmd[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
	else
		update_pwd_variables(ms);
	free(path);
}
