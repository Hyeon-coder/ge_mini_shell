/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:30:12 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

static void	execute_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_ast_node	*ast;

	tokens = lexer(line);
	if (!tokens)
    {
        // Lexer 실패 시(예: 닫히지 않은 따옴표) g_exit_status 반영
        shell->last_exit_status = g_exit_status;
		return ;
    }
	ast = parser(&tokens);
	if (!ast)
	{
        // Parser 실패 시(구문 오류) g_exit_status 반영
		cleanup(tokens, NULL);
        shell->last_exit_status = g_exit_status;
		return ;
	}
	expand_ast(ast, shell);
    // executor 내부에서 '_' 업데이트 및 실행 수행
	shell->last_exit_status = executor(ast, shell);
    g_exit_status = shell->last_exit_status; // 실행 후 전역 변수 동기화
	cleanup(tokens, ast);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		setup_signals();
        g_exit_status = 0; // 다음 명령어를 위해 초기화
		line = readline("minishell$ ");
		if (!line)
		{
            // Ctrl+D (EOF) 처리
			ft_putendl_fd("exit", 1);
			break ;
		}

        // readline 중 Ctrl+C 입력 시 상태 반영
        if (g_exit_status == 1)
            shell->last_exit_status = 1;

		if (*line)
		{
			add_history(line);
			execute_line(line, shell);
		}
		free(line);
	}
}

// non_interactive_mode, main 함수는 큰 변경 없음
void	non_interactive_mode(t_shell *shell)
{
	char	buffer[4096];
	ssize_t	bytes_read;
	char	*line;

	bytes_read = read(STDIN_FILENO, buffer, 4095);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
			buffer[bytes_read - 1] = '\0';
		line = ft_strdup(buffer);
		if (line)
		{
			execute_line(line, shell);
			free(line);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	g_exit_status = 0;
	shell.last_exit_status = 0;
	shell.env_list = init_env(envp);
	if (!shell.env_list)
	{
		print_error("Environment initialization failed", NULL, 1);
		return (1);
	}
	if (isatty(STDIN_FILENO))
		shell_loop(&shell);
	else
		non_interactive_mode(&shell);
	free_env_list(shell.env_list);
	rl_clear_history();
	return (shell.last_exit_status);
}
