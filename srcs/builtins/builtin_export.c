/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_export.c                                   :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 13:30:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_and_print_env(char **envp)
{
	int		i;
	int		j;
	char	*tmp;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(envp[j], envp[j + 1]) > 0)
			{
				tmp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(envp[i], 1);
		ft_putendl_fd("\"", 1);
		i++;
	}
}

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

static void	add_or_update_env(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*eq_ptr;

	eq_ptr = ft_strchr(arg, '=');
	if (eq_ptr)
	{
		key = ft_substr(arg, 0, eq_ptr - arg);
		value = ft_strdup(eq_ptr + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	if (!is_valid_identifier(key))
	{
		print_error("export", "not a valid identifier", 1);
		free(key);
		if (value)
			free(value);
		return ;
	}
	set_env_value(&(shell->env_list), key, value);
	free(key);
	if (value)
		free(value);
}

int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	**envp;

	if (!args[1])
	{
		envp = env_list_to_array(shell->env_list);
		if (envp)
		{
			sort_and_print_env(envp);
			free_string_array(envp);
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		add_or_update_env(args[i], shell);
		i++;
	}
	return (0);
}
