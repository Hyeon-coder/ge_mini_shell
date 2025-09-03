/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:13:45 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/03 21:13:47 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	append_dotdot_to_pwd(t_ms *ms)
{
	char	*current_pwd;
	char	*new_pwd;

	current_pwd = find_var(ms, ms->envp, "PWD");
	if (!current_pwd)
		return ;
	new_pwd = ft_strjoin(current_pwd, "/..");
	if (!new_pwd)
		return ;
	set_env_var(ms, "PWD", new_pwd);
	free(new_pwd);
}

/*
** Updates the PWD and OLDPWD environment variables after a successful cd.
*/
void	update_pwd_variables(t_ms *ms)
{
	char	cwd_buffer[1024];
	char	*old_pwd;

	old_pwd = find_var(ms, ms->envp, "PWD");
	if (old_pwd)
		set_env_var(ms, "OLDPWD", old_pwd);
	if (!getcwd(cwd_buffer, sizeof(cwd_buffer)))
	{
		if (errno == ENOENT)
		{
			append_dotdot_to_pwd(ms);
			ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
			ft_putstr_fd("cannot access parent directories: ", 2);
			ft_putendl_fd("No such file or directory", 2);
		}
	}
	else
		set_env_var(ms, "PWD", cwd_buffer);
}

/*
** Determines the target path for the 'cd' command.
*/
char	*get_target_path(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	char	*home;

	path = cmd->full_cmd[1];
	if (!path || ft_strcmp(path, "~") == 0)
		return (get_path_from_env(ms, "HOME", "minishell: cd: HOME not set"));
	if (ft_strcmp(path, "-") == 0)
		return (get_path_from_env(ms, "OLDPWD",
				"minishell: cd: OLDPWD not set"));
	if (ft_strncmp(path, "~/", 2) == 0)
	{
		home = find_var(ms, ms->envp, "HOME");
		if (!home)
			return (ft_strdup(path));
		return (ft_strjoin(home, path + 1));
	}
	return (ft_strdup(path));
}
