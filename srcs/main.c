/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 22:30:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

// execute_line 함수는 그대로 유지합니다.
static void	execute_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_token		*tokens_head;
	t_ast_node	*ast;
	char		*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t\n");
	if (!trimmed_line || !*trimmed_line)
	{
		free(trimmed_line);
		return ;
	}
	tokens = lexer(trimmed_line);
	tokens_head = tokens;
	if (!tokens)
	{
		shell->last_exit_status = g_exit_status;
		free(trimmed_line);
		return ;
	}
	ast = parser(&tokens);
	if (!ast)
	{
		cleanup(tokens_head, NULL);
		shell->last_exit_status = g_exit_status;
		free(trimmed_line);
		return ;
	}
	expand_ast(ast, shell);
	shell->last_exit_status = executor(ast, shell);
	g_exit_status = shell->last_exit_status;
	cleanup(tokens_head, ast);
	free(trimmed_line);
}


// shell_loop와 non_interactive_mode를 삭제하고 main 함수를 아래와 같이 변경합니다.
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)argc;
	(void)argv;
	g_exit_status = 0;
	shell.last_exit_status = 0;
	shell.env_list = init_env(envp);
	if (!shell.env_list)
	{
		ft_putstr_fd("minishell: Environment initialization failed\n", 2);
		return (1);
	}
	// 대화형/비대화형 모두 처리하는 단일 루프
	while (1)
	{
		setup_signals();
		line = readline("minishell$ ");
		if (!line) // EOF (ctrl-D 또는 파이프의 끝) 감지
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (g_exit_status == 130) // ctrl-C 인터럽트 처리
		{
			shell.last_exit_status = 130;
			g_exit_status = 0;
		}
		if (*line) // 입력된 라인이 비어있지 않은 경우
		{
			add_history(line);
			execute_line(line, &shell);
		}
		free(line);
	}
	free_env_list(shell.env_list);
	rl_clear_history();
	return (shell.last_exit_status);
}
