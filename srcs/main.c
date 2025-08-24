/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
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

void	non_interactive_mode(t_shell *shell)
{
	char	*line;
	char	*trimmed_line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		trimmed_line = ft_strtrim(line, "\n");
		free(line);
		if (trimmed_line && *trimmed_line)
		{
			execute_line(trimmed_line, shell);
		}
		free(trimmed_line);
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
