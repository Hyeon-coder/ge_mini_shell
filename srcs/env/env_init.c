/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* env_init.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

static void	add_env_node(t_env **head, t_env *new)
{
	t_env	*current;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	int		i;
	char	*eq_ptr;
	char	*key;
	char	*value;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		eq_ptr = ft_strchr(envp[i], '=');
		if (eq_ptr)
		{
			key = ft_substr(envp[i], 0, eq_ptr - envp[i]);
			value = ft_strdup(eq_ptr + 1);
			add_env_node(&head, new_env_node(key, value));
		}
		i++;
	}
	return (head);
}
