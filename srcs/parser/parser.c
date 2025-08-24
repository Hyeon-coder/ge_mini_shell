/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parser.c                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 17:00:00 by your_login       ###   ########.fr       */
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
	if (arg_count == -1)
	{
		g_exit_status = 2;
		return (NULL);
	}
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

static int	check_syntax_errors(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (1);
	}
	while (tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			g_exit_status = 2;
			return (1);
		}
		if ((tokens->type >= TOKEN_REDIR_IN && tokens->type <= TOKEN_HEREDOC)
			&& tokens->next->type != TOKEN_WORD)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			g_exit_status = 2;
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

t_ast_node	*parser(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;

	if (check_syntax_errors(*tokens))
		return (NULL);
	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		consume_token(tokens);
		if ((*tokens)->type == TOKEN_EOF)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			g_exit_status = 2;
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
