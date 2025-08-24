/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 13:00:00 by your_login       ###   ########.fr       */
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
		return ;
	ast = parser(&tokens);
	if (!ast)
	{
		cleanup(tokens, NULL);
		return ;
	}
	expand_ast(ast, shell);
	shell->last_exit_status = executor(ast, shell);
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
