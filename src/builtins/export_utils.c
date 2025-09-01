/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:30:32 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/02 01:30:54 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* export_utils.c */
#include "minishell.h"

/**
 * Duplicates and sorts the environment variable array.
 */
char	**duplicate_and_sort_env(t_ms *ms)
{
	char	**env_copy;
	int		i;

	env_copy = malloc(sizeof(char *) * (ms->elements + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < ms->elements)
	{
		env_copy[i] = ft_strdup(ms->envp[i]);
		if (!env_copy[i])
		{
			ft_free_array(env_copy);
			return (NULL);
		}
		i++;
	}
	env_copy[i] = NULL;
	sort_env_array(env_copy, ms->elements);
	return (env_copy);
}

/**
 * Prints a single environment variable in the 'declare -x' format.
 */
void	print_formatted_variable(char *var)
{
	char	*eq_sign;

	eq_sign = ft_strchr(var, '=');
	ft_putstr_fd("declare -x ", 1);
	if (eq_sign)
	{
		write(1, var, eq_sign - var + 1);
		ft_putchar_fd('"', 1);
		ft_putstr_fd(eq_sign + 1, 1);
		ft_putendl_fd("\"", 1);
	}
	else
		ft_putendl_fd(var, 1);
}
