/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 18:00:00 by juhyeonl         ###   ########.fr       */
/* */
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
		shell->last_exit_status = g_exit_status;
		return ;
	}
	ast = parser(&tokens);
	if (!ast)
	{
		cleanup(tokens, NULL);
		shell->last_exit_status = g_exit_status;
		return ;
	}
	expand_ast(ast, shell);
	shell->last_exit_status = executor(ast, shell);
	g_exit_status = shell->last_exit_status;
	cleanup(tokens, ast);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		setup_signals();
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (g_exit_status == 130)
		{
			shell->last_exit_status = 130;
			g_exit_status = 0;
		}
		if (*line)
		{
			add_history(line);
			execute_line(line, shell);
		}
		free(line);
	}
}

// 여러 줄의 입력을 처리하도록 수정
void	non_interactive_mode(t_shell *shell)
{
	char	*buffer;
	char	**lines;
	int		i;
	char	*tmp;

	buffer = (char *)malloc(4097);
	if (!buffer)
		return ;
	i = read(STDIN_FILENO, buffer, 4096);
	if (i > 0)
	{
		buffer[i] = '\0';
		// 개행 문자를 기준으로 모든 라인을 분리
		lines = ft_split(buffer, '\n');
		free(buffer);
		if (!lines)
			return ;
		i = 0;
		while (lines[i])
		{
			// 각 라인을 실행
			tmp = ft_strtrim(lines[i], " \t");
			execute_line(tmp, shell);
			free(tmp);
			i++;
		}
		free_string_array(lines);
	}
	else
		free(buffer);
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
		// print_error 함수 시그니처 변경에 따른 수정
		ft_putstr_fd("minishell: Environment initialization failed\n", 2);
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
