/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:29:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
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

// [신규 추가] SHLVL 업데이트 함수 (테스트 5, 24 해결)
static void update_shlvl(t_env *head)
{
    t_env   *node = head;
    int     level = 1;
    char    *new_value_str;

    // 기존 SHLVL 노드 찾기
    while (node)
    {
        if (ft_strcmp(node->key, "SHLVL") == 0)
        {
            if (node->value)
                level = ft_atoi(node->value) + 1;
            break;
        }
        node = node->next;
    }

    if (level < 0) level = 0; // 음수 처리

    new_value_str = ft_itoa(level);
    if (!new_value_str) return;

    if (node)
    {
        // 기존 노드 업데이트
        free(node->value);
        node->value = new_value_str;
    }
    else
    {
        // 새 SHLVL 노드 추가
        add_env_node(&head, new_env_node(ft_strdup("SHLVL"), new_value_str));
    }
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
	while (envp && envp[i])
	{
		eq_ptr = ft_strchr(envp[i], '=');
		if (eq_ptr)
		{
			key = ft_substr(envp[i], 0, eq_ptr - envp[i]);
			value = ft_strdup(eq_ptr + 1);
            if (key && value)
			    add_env_node(&head, new_env_node(key, value));
            else
            {
                free(key);
                free(value);
            }
		}
		i++;
	}
    update_shlvl(head); // SHLVL 업데이트 호출
	return (head);
}
