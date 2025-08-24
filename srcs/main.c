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


int	main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	char			*line;
	char			*prompt;
	struct termios	term; // 터미널 속성 저장을 위한 구조체

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
	if (isatty(STDIN_FILENO))
		prompt = "minishell$ ";
	else
	{
		prompt = "";
		// 비대화형 모드일 때 터미널의 ECHO 속성을 끕니다.
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	while (1)
	{
		setup_signals();
		line = readline(prompt);
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (g_exit_status == 130)
		{
			shell.last_exit_status = 130;
			g_exit_status = 0;
		}
		if (*line)
		{
			if (isatty(STDIN_FILENO))
				add_history(line);
			execute_line(line, &shell);
		}
		free(line);
	}
	free_env_list(shell.env_list);
	if (isatty(STDIN_FILENO))
		rl_clear_history();
	return (shell.last_exit_status);
}
