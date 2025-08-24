/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parser_utils.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	count_args_in_cmd(t_token **tokens)
{
	t_token	*current;
	int		count;

	current = *tokens;
	count = 0;
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			count++;
		else if (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			if (current->next && current->next->type == TOKEN_WORD)
				current = current->next;
			else
			{
				print_error("syntax error near unexpected token", NULL, 258);
				return (-1);
			}
		}
		current = current->next;
	}
	return (count);
}

void	consume_token(t_token **tokens)
{
	if (!tokens || !*tokens)
		return ;
	*tokens = (*tokens)->next;
}

static void	add_redir_to_node(t_ast_node *node, t_redir *new_redir)
{
	t_redir	*current;

	if (!node->redirs)
	{
		node->redirs = new_redir;
		return ;
	}
	current = node->redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static int	handle_redir_token(t_ast_node *node, t_token **tokens)
{
	t_redir	*redir;
	t_token_type redir_type; // 타입만 저장

	redir_type = (*tokens)->type; // 현재 토큰의 타입을 저장
	consume_token(tokens); // 리디렉션 토큰 (<, > 등)을 건너뜀
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		print_error("syntax error near unexpected token", NULL, 258);
		return (0);
	}
	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (0);
	redir->type = redir_type;
	redir->filename = ft_strdup((*tokens)->value);
	redir->next = NULL;
	add_redir_to_node(node, redir);
	consume_token(tokens); // 파일명 토큰을 건너뜀
	return (1);
}

void	fill_args_and_redirs(t_ast_node *node, t_token **tokens)
{
	int	i;

	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE
		&& (*tokens)->type != TOKEN_EOF)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			node->args[i] = ft_strdup((*tokens)->value);
			consume_token(tokens);
			i++;
		}
		else if (!handle_redir_token(node, tokens))
		{
			free_ast(node);
			node = NULL;
			break ;
		}
	}
}
