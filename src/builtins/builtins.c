/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 01:05:12 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 01:56:56 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Executes the 'pwd' builtin command.
** Prints the current working directory.
*/
void	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return ;
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
}

/*
** Executes the 'echo' builtin command.
** Prints the arguments to standard output.
** Handles the '-n' option to suppress the trailing newline.
*/
void	builtin_echo(t_cmd *cmd)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	if (cmd->full_cmd[i] && ft_strcmp(cmd->full_cmd[i], "-n") == 0)
	{
		newline = false;
		i++;
	}
	while (cmd->full_cmd[i])
	{
		ft_putstr_fd(cmd->full_cmd[i], 1);
		if (cmd->full_cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
}

/*
** Determines the target path for the 'cd' command based on its arguments.
** Handles cases for 'cd', 'cd ~', 'cd -', and 'cd ~/path'.
*/
static char	*get_target_path(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	char	*home;

	path = cmd->full_cmd[1];
	if (!path || ft_strcmp(path, "~") == 0)
	{
		path = find_var(ms, ms->envp, "HOME");
		if (!path)
			ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (path);
	}
	if (ft_strcmp(path, "-") == 0)
	{
		path = find_var(ms, ms->envp, "OLDPWD");
		if (!path)
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (path);
	}
	if (ft_strncmp(path, "~/", 2) == 0)
	{
		home = find_var(ms, ms->envp, "HOME");
		if (!home)
			return (NULL);
		return (ft_strjoin(home, path + 1));
	}
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
	if (cmd->full_cmd[1] && ft_strncmp(cmd->full_cmd[1], "~/", 2) == 0)
		free(path);
	else if (cmd->full_cmd[1] && ft_strcmp(cmd->full_cmd[1], path) != 0)
		;
	else if (cmd->full_cmd[1])
		free(path);
}

/*
** Executes the 'env' builtin command.
** Prints the environment variables.
** Only variables with a value (containing '=') are printed.
*/
void	builtin_env(t_ms *ms, t_cmd *cmd)
{
	int	i;

	if (cmd->full_cmd[1])
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(cmd->full_cmd[1], 2);
		ft_putendl_fd("’: No such file or directory", 2);
		return ;
	}
	i = 0;
	while (ms->envp[i])
	{
		if (ft_strchr(ms->envp[i], '='))
			ft_putendl_fd(ms->envp[i], 1);
		i++;
	}
}

/*
** Finds the index of a variable in the environment list.
*/
static int	find_var_index(char **envp, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0
			&& (envp[i][len] == '=' || envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

/*
** Removes a single environment variable at a given index by reallocating
** the envp array.
*/
static void	remove_env_var(t_ms *ms, int idx_to_remove)
{
	char	**new_envp;
	int		i;
	int		j;

	new_envp = malloc(sizeof(char *) * ms->elements);
	if (!new_envp)
		return ; // TODO: Proper error handling
	i = 0;
	j = 0;
	while (i < ms->elements)
	{
		if (i != idx_to_remove)
		{
			new_envp[j] = ms->envp[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	free(ms->envp[idx_to_remove]);
	free(ms->envp);
	ms->envp = new_envp;
	ms->elements--;
}

/*
** Executes the 'unset' builtin command.
** Loops through arguments and removes each specified environment variable.
*/
void	builtin_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;
	int	target_idx;

	i = 1;
	while (cmd->full_cmd[i])
	{
		// TODO: Add validation for the key format.
		target_idx = find_var_index(ms->envp, cmd->full_cmd[i]);
		if (target_idx != -1)
			remove_env_var(ms, target_idx);
		i++;
	}
}
