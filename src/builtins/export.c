/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:15:15 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/02 01:32:11 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Sorts a duplicated array of environment variables alphabetically.
*/
static void	sort_env_array(char **env_copy, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				tmp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

/*
** Prints all environment variables in the 'declare -x' format, sorted.
*/
static void	print_exported_vars(t_ms *ms)
{
	char	**env_copy;
	int		i;

	env_copy = duplicate_and_sort_env(ms);
	if (!env_copy)
		return ;
	i = 0;
	while (env_copy[i])
	{
		print_formatted_variable(env_copy[i]);
		i++;
	}
	ft_free_array(env_copy);
}

/*
** Checks if the key for an environment variable is a valid identifier.
** Must start with a letter or '_', followed by letters, digits, or '_'.
*/
static bool	is_valid_identifier(const char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (false);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/*
** Parses a single argument to 'export', validates it, and sets the variable.
*/
static void	process_export_argument(t_ms *ms, const char *arg)
{
	char	*key;
	char	*value;
	int		key_len;

	key_len = 0;
	while (arg[key_len] && arg[key_len] != '=')
		key_len++;
	key = ft_substr(arg, 0, key_len);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		ms->exit_status = 1;
	}
	else if (arg[key_len] == '=')
	{
		value = (char *)arg + key_len + 1;
		set_env_var(ms, key, value);
	}
	free(key);
}

/*
** Executes the 'export' builtin command.
*/
void	builtin_export(t_ms *ms, t_cmd *cmd)
{
	int	i;

	if (!cmd->full_cmd[1])
	{
		print_exported_vars(ms);
		return ;
	}
	i = 1;
	while (cmd->full_cmd[i])
	{
		process_export_argument(ms, cmd->full_cmd[i]);
		i++;
	}
}
