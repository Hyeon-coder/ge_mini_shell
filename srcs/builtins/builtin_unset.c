/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_unset.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 12:25:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_unset_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
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
			print_error("unset", "not a valid identifier", 1);
			status = 1;
		}
		else
			unset_env_var(&(shell->env_list), args[i]);
		i++;
	}
	return (status);
}
