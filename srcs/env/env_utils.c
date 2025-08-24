/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* env_utils.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	current = (t_env *)malloc(sizeof(t_env));
	if (!current)
		return ;
	current->key = ft_strdup(key);
	current->value = ft_strdup(value);
	current->next = *env_list;
	*env_list = current;
}

char	**env_list_to_array(t_env *env_list)
{
	t_env	*current;
	char	**envp;
	int		count;
	int		i;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	envp = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		envp[i] = ft_strjoin_three(current->key, "=", current->value);
		i++;
		current = current->next;
	}
	return (envp);
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}
