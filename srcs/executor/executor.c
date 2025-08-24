/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/* +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_ast_node *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));
	else
		return (execute_simple_command(node, shell));
}
