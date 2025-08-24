/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_env.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_shell *shell)
{
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(current->value, 1);
		}
		current = current->next;
	}
	return (0);
}
