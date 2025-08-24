/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parser.c                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static t_ast_node	*parse_simple_command(t_token **tokens)
{
	t_ast_node	*node;
	int			arg_count;

	if (!tokens || !(*tokens) || (*tokens)->type == TOKEN_EOF)
		return (NULL);
	arg_count = count_args_in_cmd(tokens);
	node = new_ast_node(NODE_CMD);
	if (!node)
		return (NULL);
	node->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
	if (!node->args)
	{
		free_ast(node);
		return (NULL);
	}
	fill_args_and_redirs(node, tokens);
	return (node);
}

static t_ast_node	*build_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	if (!left || !right)
	{
		if (left)
			free_ast(left);
		if (right)
			free_ast(right);
		return (NULL);
	}
	node = new_ast_node(NODE_PIPE);
	if (!node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	node->left = left;
	node->right = right;
	return (node);
}

t_ast_node	*parser(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		consume_token(tokens);
		if ((*tokens)->type == TOKEN_EOF || (*tokens)->type == TOKEN_PIPE)
		{
			print_error("syntax error near unexpected token", "|", 258);
			free_ast(left);
			return (NULL);
		}
		right = parser(tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_pipe_node(left, right);
	}
	return (left);
}
