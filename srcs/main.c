/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 19:30:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

static void	execute_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_ast_node	*ast;
	char		*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t\n");
	if (!trimmed_line || !*trimmed_line)
	{
		free(trimmed_line);
		return ;
	}
	tokens = lexer(trimmed_line);
	if (!tokens)
	{
		shell->last_exit_status = g_exit_status;
		free(trimmed_line);
		return ;
	}
	ast = parser(&tokens);
	if (!ast)
	{
		cleanup(tokens, NULL);
		shell->last_exit_status = g_exit_status;
		free(trimmed_line);
		return ;
	}
	expand_ast(ast, shell);
	shell->last_exit_status = executor(ast, shell);
	g_exit_status = shell->last_exit_status;
	cleanup(tokens, ast);
	free(trimmed_line);
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

void	non_interactive_mode(t_shell *shell)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		execute_line(line, shell);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}

/*
non_interactive_mode
	ex) echo "echo abc" | ./minishell	=> it will be false by isatty()
Then, how about true by isatty()?		=> shell_loop()
	ex) echo abc
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		is_interactive; // 인터랙티브 모드 여부를 저장할 변수

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
	is_interactive = isatty(STDIN_FILENO); // 모드를 미리 확인
	if (is_interactive)
		shell_loop(&shell);
	else
		non_interactive_mode(&shell);
	free_env_list(shell.env_list);
	if (is_interactive) // 대화형 모드일 때만 호출
		rl_clear_history();
	return (shell.last_exit_status);
}
