/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:53:52 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 10:59:42 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Sorts the environment variable array alphabetically.
*/
static void	bubble_sort_env(char **envp, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/*
** Prints the sorted environment variables in the 'export' format.
*/
static void	print_exported_env(char **envp)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "_=", 2) != 0)
		{
			ft_putstr_fd("export ", 1);
			j = 0;
			while (envp[i][j] && envp[i][j] != '=')
				ft_putchar_fd(envp[i][j++], 1);
			if (envp[i][j] == '=')
			{
				ft_putchar_fd('=', 1);
				ft_putchar_fd('"', 1);
				ft_putstr_fd(&envp[i][j + 1], 1);
				ft_putchar_fd('"', 1);
			}
			ft_putchar_fd('\n', 1);
		}
		i++;
	}
}

/*
** Converts the environment list to an array, sorts it, and prints it.
*/
static void	sort_and_print_env(t_shell *shell)
{
	char	**envp;
	int		count;

	envp = env_list_to_array(shell->env_list);
	if (!envp)
		return ;
	count = 0;
	while (envp[count])
		count++;
	bubble_sort_env(envp, count);
	print_exported_env(envp);
	free_string_array(envp);
}

/*
** Checks if a string is a valid identifier for an environment variable.
*/
static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
** Parses the argument, validates it, and adds or updates the environment
** variable.
*/
static int	add_or_update_env(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*eq_ptr;

	eq_ptr = ft_strchr(arg, '=');
	if (eq_ptr)
		key = ft_substr(arg, 0, eq_ptr - arg);
	else
		key = ft_strdup(arg);
	if (!key || !is_valid_identifier(key))
	{
		print_error("export", \
			ft_strjoin_free(ft_strjoin("`", arg), \
			"': not a valid identifier"), 1);
		free(key);
		return (1);
	}
	if (eq_ptr)
		value = ft_strdup(eq_ptr + 1);
	else
		value = NULL;
	set_env_value(&(shell->env_list), key, value);
	free(key);
	free(value);
	return (0);
}

/*
** Sets environment variables. With no arguments, it prints all environment
** variables. Otherwise, it sets the variables provided in the arguments.
*/
int	builtin_export(char **args, t_shell *shell)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
	{
		sort_and_print_env(shell);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (add_or_update_env(args[i], shell) != 0)
			status = 1;
		i++;
	}
	return (status);
}
