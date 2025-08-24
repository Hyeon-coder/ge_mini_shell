/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 16:19:40 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 내부 실행 함수 (헤더 파일에 선언 필요)
int	_executor(t_ast_node *node, t_shell *shell, int is_child);

// '_' 변수 업데이트를 위한 마지막 인자 찾기 (테스트 5, 24)
static void update_underscore_var(t_ast_node *node, t_shell *shell)
{
    t_ast_node *last_cmd = node;
    char *last_arg = NULL;
    int i;

    if (!node) return;

    // 파이프라인의 마지막 명령어 찾기 (가장 오른쪽 노드)
    while (last_cmd->type == NODE_PIPE && last_cmd->right)
        last_cmd = last_cmd->right;

    // 마지막 명령어의 마지막 인자 찾기
    if (last_cmd->type == NODE_CMD && last_cmd->args)
    {
        i = 0;
        while (last_cmd->args[i])
            i++;
        if (i > 0)
            last_arg = last_cmd->args[i - 1];
    }

    if (last_arg)
    {
        // 주의: set_env_value 함수가 env_utils.c에 구현되어 있어야 합니다.
        set_env_value(&(shell->env_list), "_", last_arg);
    }
}

// 내부 실행 로직 (컨텍스트 전달)
int	_executor(t_ast_node *node, t_shell *shell, int is_child)
{
	if (!node)
		return (0);

	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));
	else
        // 단순 명령어 실행 시 컨텍스트(is_child) 전달
		return (execute_simple_command(node, shell, is_child));
}

// 메인 실행기
int	executor(t_ast_node *node, t_shell *shell)
{
    update_underscore_var(node, shell);
    // 최상위 실행은 is_child = 0
    return (_executor(node, shell, 0));
}
