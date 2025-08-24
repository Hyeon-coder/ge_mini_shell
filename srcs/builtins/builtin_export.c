/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_export.c                                   :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 18:30:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_and_print_env(t_shell *shell)
{
	char	**envp;
	int		i;
	int		j;
	char	*tmp;
	int		count;

	envp = env_list_to_array(shell->env_list);
	if (!envp)
		return ;
	count = 0;
	while (envp[count])
		count++;
	i = -1;
	while (++i < count)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
		}
	}
	i = -1;
	while (++i < count)
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
	}
	free_string_array(envp);
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
		print_error("export", arg, 1);
		free(key);
		return (1);
	}
	if (eq_ptr)
		value = ft_strdup(eq_ptr + 1);
	else
		value = NULL;
	set_env_value(&(shell->env_list), key, value);
	free(key);
	if (value)
		free(value);
	return (0);
}

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
