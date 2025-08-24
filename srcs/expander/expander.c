/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expander.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/* +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_args(t_ast_node *node, t_shell *shell)
{
	int		i;
	char	*original_arg;

	i = 0;
	while (node->args && node->args[i])
	{
		original_arg = node->args[i];
		node->args[i] = expand_string(original_arg, shell);
		free(original_arg);
		i++;
	}
}

static void	expand_redirs(t_ast_node *node, t_shell *shell)
{
	t_redir	*current;
	char	*original_filename;

	current = node->redirs;
	while (current)
	{
		original_filename = current->filename;
		current->filename = expand_string(original_filename, shell);
		free(original_filename);
		current = current->next;
	}
}

void	expand_ast(t_ast_node *node, t_shell *shell)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		expand_ast(node->left, shell);
		expand_ast(node->right, shell);
	}
	else if (node->type == NODE_CMD)
	{
		expand_args(node, shell);
		expand_redirs(node, shell);
	}
}
