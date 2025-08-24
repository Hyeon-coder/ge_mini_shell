/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ast_utils.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*new_ast_node(t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	return (node);
}

static void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs->next;
		free(redirs->filename);
		free(redirs);
		redirs = tmp;
	}
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		free_ast(node->left);
		free_ast(node->right);
	}
	if (node->args)
		free_string_array(node->args);
	if (node->redirs)
		free_redirs(node->redirs);
	free(node);
}
