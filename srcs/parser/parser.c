/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:36:54 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

/* --- 구문 검사 관련 함수 추가 --- */

static int is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
			type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

// 구문 오류 보고 및 종료 상태 2 설정 (Bash 표준)
static int report_syntax_error(t_token *token)
{
	g_exit_status = 2;
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	if (!token || token->type == TOKEN_EOF)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
		ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (1); // 에러 발생 표시
}

// 토큰 리스트 전체에 대한 사전 구문 검사 (테스트 28, 29 해결)
static int check_syntax(t_token *tokens)
{
	t_token *current = tokens;

	if (!current || current->type == TOKEN_EOF)
		return (0);

	// 1. 파이프가 처음에 오는 경우
	if (current->type == TOKEN_PIPE)
		return (report_syntax_error(current));

	while (current && current->type != TOKEN_EOF)
	{
		// 2. 리디렉션 검사 (테스트 29 세그폴트 방지)
		if (is_redirection(current->type))
		{
			// 다음 토큰이 없거나(EOF), 파일 이름(WORD)이 아닌 경우
			if (!current->next || current->next->type == TOKEN_EOF)
				return (report_syntax_error(current->next));
			if (current->next->type != TOKEN_WORD)
				return (report_syntax_error(current->next));
		}
		// 3. 파이프 검사 (테스트 28: ls | | wc)
		else if (current->type == TOKEN_PIPE)
		{
			// 다음 토큰이 없거나(EOF), 다음 토큰이 다시 파이프인 경우
			if (!current->next || current->next->type == TOKEN_EOF)
				return (report_syntax_error(current->next));
			if (current->next->type == TOKEN_PIPE)
				return (report_syntax_error(current->next));
		}
		current = current->next;
	}
	return (0);
}

/* --- 파싱 로직 --- */

static t_ast_node	*parse_simple_command(t_token **tokens)
{
	t_ast_node	*node;
	int			arg_count;

	if (!tokens || !(*tokens) || (*tokens)->type == TOKEN_EOF || (*tokens)->type == TOKEN_PIPE)
		return (NULL);

	arg_count = count_args_in_cmd(tokens);
	node = new_ast_node(NODE_CMD);
	if (!node)
		return (NULL);

	// 인자가 있을 경우에만 메모리 할당
	if (arg_count > 0)
	{
		node->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
		if (!node->args)
		{
			free_ast(node);
			return (NULL);
		}
	}
	// check_syntax를 통과했으므로 안전하게 호출 가능
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

// 파서 메인 함수 (반복적, 왼쪽 우선 결합 방식으로 변경)
t_ast_node	*parser(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;

	// [추가] 사전 구문 검사 수행
	if (check_syntax(*tokens) != 0)
		return (NULL);

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL); // 빈 입력

	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		consume_token(tokens); // 파이프 토큰 소비

		// [수정] 재귀 호출(parser) 대신 parse_simple_command 호출
		right = parse_simple_command(tokens);
		if (!right)
		{
			// check_syntax 통과 시 발생하지 않음
			free_ast(left);
			return (NULL);
		}
		left = build_pipe_node(left, right);
		if (!left)
			return (NULL); // 메모리 할당 실패
	}
	return (left);
}
