/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:54:07 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 10:54:09 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_unset_identifier(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	unset_env_var(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid_unset_identifier(args[i]))
		{
			print_error("unset", args[i], 1);
			status = 1;
		}
		else
			unset_env_var(&(shell->env_list), args[i]);
		i++;
	}
	return (status);
}
